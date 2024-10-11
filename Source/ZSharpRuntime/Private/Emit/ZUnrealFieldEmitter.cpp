// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealFieldEmitter.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ZSharpFieldRegistry.h"
#include "Algo/TopologicalSort.h"
#include "ZUnrealFieldDefinitions.h"
#include "Reflection/Function/ZFunctionVisitor.h"
#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "UObject/PropertyOptional.h"

namespace ZSharp::ZSharpClass_Private
{
	static void ConstructObject(const FObjectInitializer& objectInitializer, UClass* cls)
	{
		UObject* obj = objectInitializer.GetObj();

		const FZSharpClass* zscls = FZSharpFieldRegistry::Get().GetClass(cls);
		const bool native = cls->IsNative();
		// If we are dynamic class (Z# class or BPGC) then call super constructor first.
		if (zscls || !native)
		{
			// If we are Z# class, apply default subobject overrides before call super.
			if (zscls)
			{
				for (const auto& defaultSubobjectOverride : zscls->DefaultSubobjectOverrides)
				{
					if (defaultSubobjectOverride.ClassPath.IsNone())
					{
						objectInitializer.DoNotCreateDefaultSubobject(defaultSubobjectOverride.Name);
					}
					else if (const auto overrideClass = LoadClass<UObject>(nullptr, *defaultSubobjectOverride.ClassPath.ToString()))
					{
						objectInitializer.SetDefaultSubobjectClass(defaultSubobjectOverride.Name, overrideClass);
					}
					else
					{
						UE_LOG(LogZSharpEmit, Warning, TEXT("Default subobject override class [%s] not found! Outer: [%s] SubobjectName: [%s]"), *defaultSubobjectOverride.ClassPath.ToString(), *GetNameSafe(obj), *defaultSubobjectOverride.Name.ToString());
					}
				}
			}
			
			ConstructObject(objectInitializer, cls->GetSuperClass());
		}

		// BPGC doesn't need to do anything else.
		if (!native)
		{
			return;
		}
		
		// Setup Z# properties for Z# class.
		if (zscls)
		{
			{ // Initialize Z# memory.
				for (TFieldIterator<FProperty> it(cls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
				{
					it->InitializeValue_InContainer(obj);
				}
			}

			{ // Setup default subobjects.
				for (const auto& defaultSubobject : zscls->DefaultSubobjects)
				{
					if (const auto defaultSubobjectClass = LoadClass<UObject>(nullptr, *defaultSubobject.ClassPath.ToString()))
					{
						// Subclass may not allow us to create so subobject can be null.
						if (UObject* subobject = obj->CreateDefaultSubobject(defaultSubobject.Name, defaultSubobjectClass, defaultSubobjectClass, !defaultSubobject.bOptional, defaultSubobject.bTransient))
						{
							// Associate the subobject with a property.
							if (defaultSubobject.Property)
							{
								defaultSubobject.Property->SetObjectPropertyValue_InContainer(obj, subobject);
							}

							// Setup scene info.
							if (defaultSubobject.bRootComponent)
							{
								const auto actor = Cast<AActor>(obj);
								const auto root = Cast<USceneComponent>(subobject);
								if (ensureAlways(actor) && ensureAlways(root))
								{
									USceneComponent* prevRoot = actor->GetRootComponent();
									actor->SetRootComponent(root);
									if (prevRoot)
									{
										prevRoot->SetupAttachment(root);
									}
								}
							}
							else if (!defaultSubobject.AttachParentDefaultSubobjectName.IsNone())
							{
								ensureAlways(!defaultSubobject.bRootComponent);

								const auto child = Cast<USceneComponent>(subobject);
								const auto parent = Cast<USceneComponent>(obj->GetDefaultSubobjectByName(defaultSubobject.AttachParentDefaultSubobjectName));
								if (ensureAlways(child) && ensureAlways(parent))
								{
									child->SetupAttachment(parent, defaultSubobject.AttachSocketName);
								}
							}
						}
					}
					else
					{
						UE_LOG(LogZSharpEmit, Warning, TEXT("Default subobject class [%s] not found! Outer: [%s] SubobjectName: [%s]"), *defaultSubobject.ClassPath.ToString(), *GetNameSafe(obj), *defaultSubobject.Name.ToString());
					}
				}
			}

			{ // Setup property defaults
				for (const auto& propertyDefault : zscls->PropertyDefaults)
				{
					void* propertyAddr = obj;
					FProperty* tailProperty = propertyDefault.PropertyChain[propertyDefault.PropertyChain.Num() - 1];
					// Follow the container chain to resolve the actual container.
					for (const auto& prop : propertyDefault.PropertyChain)
					{
						if (const auto objectProp = CastField<FObjectPropertyBase>(prop))
						{
							propertyAddr = objectProp->GetObjectPropertyValue_InContainer(propertyAddr);
						}
						else
						{
							check(prop->IsA<FStructProperty>() || prop == tailProperty);
							propertyAddr = prop->ContainerPtrToValuePtr<void>(propertyAddr);
						}
					}
					
					tailProperty->ImportText_Direct(*propertyDefault.Buffer, propertyAddr, nullptr, PPF_None);
				}
			}
		}
		// Only call self constructor and stop call up because C++ itself will handle the rest.
		else
		{
			cls->ClassConstructor(objectInitializer);
		}
	}
	
	static void ClassConstructor(const FObjectInitializer& objectInitializer)
	{
		ConstructObject(objectInitializer, objectInitializer.GetClass());
	}
}

namespace ZSharp::ZSharpFunction_Private
{
	static DEFINE_FUNCTION(execZCall)
	{
		FZFunctionVisitorRegistry::Get().Get(Stack.CurrentNativeFunction)->InvokeZCall(Context, Stack, RESULT_PARAM);
	}
}

namespace ZSharp::ZUnrealFieldEmitter_Private
{
	static void FatalIfObjectExists(UObject* outer, FName name)
	{
		UE_CLOG(!!FindObject<UObject>(outer, *name.ToString()), LogZSharpEmit, Fatal, TEXT("Object [%s].[%s] already exists!!!"), *outer->GetPathName(), *name.ToString());
	}
	
	static void AddMetadata(UField* field, const TMap<FName, FString>& metadata)
	{
#if WITH_METADATA
		if (!metadata.IsEmpty())
		{
			UMetaData* target = field->GetOutermost()->GetMetaData();
			for (const auto& pair : metadata)
			{
				target->SetValue(field, pair.Key, *pair.Value);
			}
		}
#endif
	}

	static void AddMetadata(FField* field, const TMap<FName, FString>& metadata)
	{
#if WITH_METADATA
		for (const auto& pair : metadata)
		{
			field->SetMetaData(pair.Key, *pair.Value);
		}
#endif
	}

	static void FinishEmitSimpleProperty(FZSimplePropertyDefinition& def)
	{
		FProperty* property = def.Property;
		property->PropertyFlags |= def.PropertyFlags;

		// Migrate from FProperty::Init().
		if (property->GetOwner<UObject>())
		{
			const auto owner = property->GetOwnerChecked<UField>();
			owner->AddCppProperty(property);
		}
		else
		{
			const auto owner = property->GetOwnerChecked<FField>();
			owner->AddCppProperty(property);
		}
	}

	static constexpr EObjectFlags GCompiledInPropertyObjectFlags = RF_Public | RF_Transient | RF_MarkAsNative;

#define NEW_PROPERTY(PropertyTypeName) \
	auto property = new F##PropertyTypeName##Property(owner, name, flags | GCompiledInPropertyObjectFlags); \
	def.Property = property;
	
	static void EmitSimpleProperty(FFieldVariant owner, FZSimplePropertyDefinition& def, FName name, EObjectFlags flags)
	{
		switch (def.Type)
		{
			// Primitives
		case EZPropertyType::UInt8:
			{
				NEW_PROPERTY(Byte);
				break;
			}
		case EZPropertyType::UInt16:
			{
				NEW_PROPERTY(UInt16);
				break;
			}
		case EZPropertyType::UInt32:
			{
				NEW_PROPERTY(UInt32);
				break;
			}
		case EZPropertyType::UInt64:
			{
				NEW_PROPERTY(UInt64);
				break;
			}
		case EZPropertyType::Int8:
			{
				NEW_PROPERTY(Int8);
				break;
			}
		case EZPropertyType::Int16:
			{
				NEW_PROPERTY(Int16);
				break;
			}
		case EZPropertyType::Int32:
			{
				NEW_PROPERTY(Int);
				break;
			}
		case EZPropertyType::Int64:
			{
				NEW_PROPERTY(Int64);
				break;
			}
		case EZPropertyType::Float:
			{
				NEW_PROPERTY(Float);
				break;
			}
		case EZPropertyType::Double:
			{
				NEW_PROPERTY(Double);
				break;
			}
		case EZPropertyType::Enum:
			{
				NEW_PROPERTY(Enum);

				const auto underlyingProperty = new FInt64Property(property, NAME_None, GCompiledInPropertyObjectFlags);
				property->SetEnum(FindObjectChecked<UEnum>(nullptr, *def.DescriptorFieldPath.ToString()));
				property->AddCppProperty(underlyingProperty);
				
				break;
			}
			// Strings
		case EZPropertyType::String:
			{
				NEW_PROPERTY(Str);
				break;
			}
		case EZPropertyType::Name:
			{
				NEW_PROPERTY(Name);
				break;
			}
		case EZPropertyType::Text:
			{
				NEW_PROPERTY(Text);
				break;
			}
			// Object wrappers
			// @TODO: If PropertyClass has CLASS_DefaultToInstanced in hierarchy, then the property should set CPF_InstancedReference | CPF_ExportObject. (@see UhtObjectPropertyBase.cs)
		case EZPropertyType::Object:
			{
				NEW_PROPERTY(Object);

				property->PropertyFlags |= CPF_TObjectPtrWrapper; // Migrate from UHT.
				
				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::Class:
			{
				NEW_PROPERTY(Class);

				property->PropertyFlags |= CPF_TObjectPtrWrapper; // Migrate from UHT.
				
				property->PropertyClass = UClass::StaticClass();
				property->MetaClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::SoftClass:
			{
				NEW_PROPERTY(SoftClass);

				property->PropertyFlags |= CPF_UObjectWrapper; // Migrate from UHT.
				
				property->MetaClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::SoftObject:
			{
				NEW_PROPERTY(SoftObject);

				property->PropertyFlags |= CPF_UObjectWrapper; // Migrate from UHT.
				
				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
			// There seems to be a TAutoWeakObjectPtr<> and CPF_AutoWeak but I don't see...
		case EZPropertyType::WeakObject:
			{
				NEW_PROPERTY(WeakObject);

				property->PropertyFlags |= CPF_UObjectWrapper; // Migrate from UHT.
				
				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::LazyObject:
			{
				NEW_PROPERTY(LazyObject);

				property->PropertyFlags |= CPF_UObjectWrapper; // Migrate from UHT.
				
				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::Interface:
			{
				NEW_PROPERTY(Interface);

				property->PropertyFlags |= CPF_UObjectWrapper; // Migrate from UHT.
				
				property->InterfaceClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
			// Containers (Struct)
			// @TODO: If Struct has instanced reference, then the property should set CPF_ContainsInstancedReference. (@see UhtStructProperty.cs)
		case EZPropertyType::Struct:
			{
				NEW_PROPERTY(Struct);

				property->Struct = FindObjectChecked<UScriptStruct>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
			// Delegates
		case EZPropertyType::Delegate:
			{
				NEW_PROPERTY(Delegate);

				property->PropertyFlags |= CPF_InstancedReference; // Migrate from UHT.
				
				property->SignatureFunction = FindObjectChecked<UDelegateFunction>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::MulticastInlineDelegate:
			{
				NEW_PROPERTY(MulticastInlineDelegate);

				property->PropertyFlags |= CPF_InstancedReference; // Migrate from UHT.
				
				property->SignatureFunction = FindObjectChecked<UDelegateFunction>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
			// Special types
		case EZPropertyType::FieldPath:
			{
				NEW_PROPERTY(FieldPath);

				property->PropertyClass = FProperty::StaticClass();
				
				break;
			}
		default:
			{
				UE_LOG(LogZSharpEmit, Fatal, TEXT("Unknown property type [%d]!!!"), def.Type);
				break;
			}
		}

		FinishEmitSimpleProperty(def);
	}

	static void EmitProperty(FFieldVariant owner, FZPropertyDefinition& def)
	{
		const FName name = def.Name;
		const EObjectFlags flags = def.Flags;
		
		bool needsFinishSimple = true;
		switch (def.Type)
		{
			// Containers (except Struct)
		case EZPropertyType::Array:
			{
				NEW_PROPERTY(Array);

				EmitSimpleProperty(property, def.InnerProperty, name, RF_NoFlags);

				// Migrate from UHT.
				property->PropertyFlags |= property->Inner->PropertyFlags & CPF_TObjectPtrWrapper;
				property->Inner->PropertyFlags = property->PropertyFlags & CPF_PropagateToArrayInner;

				break;
			}
		case EZPropertyType::Set:
			{
				NEW_PROPERTY(Set);

				// Assume inner property is hashable.
				def.InnerProperty.PropertyFlags |= CPF_HasGetValueTypeHash;

				EmitSimpleProperty(property, def.InnerProperty, name, RF_NoFlags);

				// Migrate from UHT.
				property->PropertyFlags |= property->ElementProp->PropertyFlags & CPF_TObjectPtrWrapper;
				property->ElementProp->PropertyFlags = property->PropertyFlags & CPF_PropagateToSetElement;

				break;
			}
		case EZPropertyType::Map:
			{
				NEW_PROPERTY(Map);

				// Assume inner property is hashable.
				def.InnerProperty.PropertyFlags |= CPF_HasGetValueTypeHash;

				EmitSimpleProperty(property, def.InnerProperty, FName(FString::Printf(TEXT("%s_Key"), *name.ToString())), RF_NoFlags);
				EmitSimpleProperty(property, def.OuterProperty, name, RF_NoFlags);

				// @TODO: These should migrate from UHT but I don't really understand what it does...
				property->PropertyFlags |= property->KeyProp->PropertyFlags & CPF_TObjectPtrWrapper;
				property->PropertyFlags |= property->ValueProp->PropertyFlags & CPF_TObjectPtrWrapper;
				property->KeyProp->PropertyFlags = property->PropertyFlags & CPF_PropagateToMapKey;
				property->ValueProp->PropertyFlags = property->PropertyFlags & CPF_PropagateToMapValue;
				
				break;
			}
		case EZPropertyType::Optional:
			{
				NEW_PROPERTY(Optional);

				EmitSimpleProperty(property, def.InnerProperty, name, RF_NoFlags);

				// Migrate from UHT.
				property->PropertyFlags |= property->GetValueProperty()->PropertyFlags & CPF_TObjectPtrWrapper;
				property->GetValueProperty()->PropertyFlags = property->PropertyFlags & CPF_PropagateToOptionalInner;

				break;
			}
		default:
			{
				EmitSimpleProperty(owner, def, name, flags);
				needsFinishSimple = false;
				break;
			}
		}

		if (needsFinishSimple)
		{
			FinishEmitSimpleProperty(def);
		}

		{ // Finish emit outer property.
			FProperty* property = def.Property;
			property->RepNotifyFunc = def.RepNotifyName;
		}
	}

#undef NEW_PROPERTY

	static void EmitProperties(UStruct* outer, TArray<FZPropertyDefinition>& defs)
	{
		for (int32 i = defs.Num() - 1; i >= 0; --i)
		{
			EmitProperty(outer, defs[i]);
		}
	}

	static void EmitFunction(UClass* outer, FZFunctionDefinition& def)
	{
		// Migrate from UECodeGen_Private::ConstructUFunction().
		constexpr EObjectFlags GCompiledInFlags = RF_Public | RF_Transient | RF_MarkAsNative;
		// Z# function must be native.
		constexpr EFunctionFlags GCompiledInFunctionFlags = FUNC_Native;

		FStaticConstructObjectParameters params { UFunction::StaticClass() };
		params.Outer = outer;
		params.Name = *def.Name.ToString();
		params.SetFlags = def.Flags | GCompiledInFlags;
	
		const auto function = static_cast<UFunction*>(StaticConstructObject_Internal(params));
		def.Function = function;

		function->FunctionFlags |= def.FunctionFlags | GCompiledInFunctionFlags;
		function->RPCId = def.RpcId;
		function->RPCResponseId = def.RpcResponseId;

		outer->AddNativeFunction(*function->GetName(), ZSharpFunction_Private::execZCall);

		EmitProperties(function, def.Properties);
		AddMetadata(function, def.MetadataMap);

		if (function->HasAllFunctionFlags(FUNC_BlueprintEvent))
		{
			if (UFunction* superFunction = outer->GetSuperClass()->FindFunctionByName(function->GetFName()))
			{
				UE_CLOG(!function->IsSignatureCompatibleWith(superFunction), LogZSharpEmit, Fatal, TEXT("BlueprintEventOverride [%s] signature mismatch!"), *GetNameSafe(function));

				// Migrate from FBlueprintCompilationManager::FastGenerateSkeletonClass().
				function->SetSuperStruct(superFunction);
				function->FunctionFlags |= (superFunction->FunctionFlags & (FUNC_FuncInherit | FUNC_Public | FUNC_Protected | FUNC_Private | FUNC_BlueprintPure));
				UMetaData::CopyMetadata(superFunction, function);
			}
		}

		function->Bind();
		function->StaticLink(true);

		// Migrate from UClass::CreateLinkAndAddChildFunctionsToMap().
		function->Next = outer->Children;
		outer->Children = function;
		outer->AddFunctionToFunctionMap(function, function->GetFName());

		{ // Finally setup proxy.
			FZSharpFunction& zsfunction = FZSharpFieldRegistry::Get().RegisterFunction(function);
			zsfunction.ZCallName = def.ZCallName;
		}
	}

	static void EmitFunctions(UClass* outer, TArray<FZFunctionDefinition>& defs)
	{
		for (int32 i = defs.Num() - 1; i >= 0; --i)
		{
			EmitFunction(outer, defs[i]);
		}
	}
}

ZSharp::FZUnrealFieldEmitter& ZSharp::FZUnrealFieldEmitter::Get()
{
	static FZUnrealFieldEmitter GSingleton;

	return GSingleton;
}

void ZSharp::FZUnrealFieldEmitter::Emit(FZUnrealFieldManifest& manifest)
{
	UE_CLOG(bEmitting, LogZSharpEmit, Fatal, TEXT("Emit is a package-wide atomic operation so can't invoke recursively!!!"));
	TGuardValue recursionGuard { bEmitting, true };

	if (EmittedModules.Contains(manifest.ModuleName))
	{
		return;
	}
	EmittedModules.Emplace(manifest.ModuleName);
	
	InternalEmit(manifest);
}

void ZSharp::FZUnrealFieldEmitter::InternalEmit(FZUnrealFieldManifest& manifest) const
{
	if (manifest.Enums.IsEmpty() && manifest.Structs.IsEmpty() && manifest.Classes.IsEmpty() && manifest.Interfaces.IsEmpty() && manifest.Delegates.IsEmpty())
	{
		return;
	}
	
	// Migrate from UObjectBase::DeferredRegister().
	const FString packageName = FString::Printf(TEXT("/Script/%s"), *manifest.ModuleName.ToString());
	UPackage* pak = CreatePackage(*packageName);
	manifest.Package = pak;

	pak->SetPackageFlags(PKG_CompiledIn);

	// Enums have no dependency so we can emit them just one step.
	for (auto& def : manifest.Enums)
	{
		EmitEnum(pak, def);
	}
	
	// Other fields have dependency so we emit skeleton (placeholder) first.
	for (auto& def : manifest.Structs)
	{
		EmitStructSkeleton(pak, def);
	}

	for (auto& def : manifest.Classes)
	{
		EmitClassSkeleton(pak, def);
	}

	for (auto& def : manifest.Interfaces)
	{
		EmitInterfaceSkeleton(pak, def);
	}

	for (auto& def : manifest.Delegates)
	{
		EmitDelegateSkeleton(pak, def);
	}

	// Now that all fields are in memory (but not fully initialized) and we can setup dependency.
	// IMPORTANT: Structs must finish before others because properties depend on structs need to calculate size.
	// @TODO: Sort structs by reference.
	for (auto& def : manifest.Structs)
	{
		FinishEmitStruct(pak, def);
	}

	// Subclass may depend on super/within class's data i.e. ClassConfigName, SparseClassDataStruct.
	// So we sort classes to ensure super/within class get constructed before subclass.
	// Algo::TopologicalSort doesn't support projection or custom hash, so we manually make a projection.
	{
		TArray<const UClass*> classes;
		TMap<const UClass*, FZClassDefinition*> map;
		for (auto& def : manifest.Classes)
		{
			classes.Emplace(def.Class);
			map.Emplace(def.Class, &def);
		}
		
		Algo::TopologicalSort(classes, [&map](const UClass* cls)
		{
			const FZClassDefinition* classDef = map.FindChecked(cls);
			TArray<UClass*, TInlineAllocator<2>> dependencies;
			
			if (const auto superClass = FindObjectChecked<UClass>(nullptr, *classDef->SuperPath.ToString()); FZSharpFieldRegistry::Get().IsZSharpClass(superClass))
			{
				dependencies.Emplace(superClass);
			}
			
			if (!classDef->WithinPath.IsNone())
			{
				if (const auto withinClass = FindObjectChecked<UClass>(nullptr, *classDef->WithinPath.ToString()); FZSharpFieldRegistry::Get().IsZSharpClass(withinClass))
				{
					dependencies.Emplace(withinClass);
				}
			}
			
			return dependencies;
		});
		
		for (auto cls : classes)
		{
			FZClassDefinition* classDef = map.FindChecked(cls);
			FinishEmitClass(pak, *classDef);
		}
	}

	for (auto& def : manifest.Interfaces)
	{
		FinishEmitInterface(pak, def);
	}

	for (auto& def : manifest.Delegates)
	{
		FinishEmitDelegate(pak, def);
	}

	for (auto& def : manifest.Classes)
	{
		PostEmitClass(pak, def);
	}
}

void ZSharp::FZUnrealFieldEmitter::EmitEnum(UPackage* pak, FZEnumDefinition& def) const
{
	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::EmitStructSkeleton(UPackage* pak, FZScriptStructDefinition& def) const
{
	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::EmitClassSkeleton(UPackage* pak, FZClassDefinition& def) const
{
	// Migrate from GetPrivateStaticClassBody().
	constexpr EObjectFlags GCompiledInFlags = RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet;
	// Migrate from static constructor of UClass.
	constexpr EClassFlags GCompiledInClassFlags = CLASS_Native;

	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);
	
	FStaticConstructObjectParameters params { UClass::StaticClass() };
	params.Outer = pak;
	params.Name = *def.Name.ToString();
	params.SetFlags = def.Flags | GCompiledInFlags;
	
	const auto cls = static_cast<UClass*>(StaticConstructObject_Internal(params));
	def.Class = cls;

	// Migrate from UClass(EStaticConstructor).
	// UZSharpClass has no ClassVTableHelperCtorCaller and CppClassStaticFunctions so will directly copy from super in Bind().
	cls->ClassConstructor = ZSharpClass_Private::ClassConstructor;
	cls->ClassUnique = 0;
	cls->bCooked = false;
	cls->bLayoutChanging = false;
	cls->ClassFlags = def.ClassFlags | GCompiledInClassFlags;
	cls->ClassCastFlags = def.CastFlags;
#if WITH_EDITORONLY_DATA
	cls->ClassGeneratedBy = nullptr; // UZSharpClass doesn't depend on blueprint.
	cls->PropertiesPendingDestruction = nullptr;
#endif
	// @TODO: UZSharpClass is not support NetFields yet.
	cls->ClassDefaultObject = nullptr;
	cls->SetSparseClassDataStruct(nullptr);
	
	// Migrate from UECodeGen_Private::ConstructUClass().
	FCppClassTypeInfoStatic typeInfo;
	typeInfo.bIsAbstract = cls->HasAllClassFlags(CLASS_Abstract);
	cls->SetCppTypeInfoStatic(&typeInfo);
	cls->ClassConfigName = def.ConfigName;

	{ // Finally setup proxy.
		FZSharpClass& zscls = FZSharpFieldRegistry::Get().RegisterClass(cls);

		// Property defaults will be constructed in PostEmitClass().
		
        zscls.DefaultSubobjects.Reserve(def.DefaultSubobjects.Num());
        for (const auto& defaultSubobjectDef : def.DefaultSubobjects)
        {
        	FZSharpClass::FDefaultSubobject& defaultSubobject = zscls.DefaultSubobjects.Emplace_GetRef();
        	defaultSubobject.Name = defaultSubobjectDef.Name;
        	defaultSubobject.ClassPath = defaultSubobjectDef.ClassPath;
        	defaultSubobject.bOptional = defaultSubobjectDef.bOptional;
        	defaultSubobject.bTransient = defaultSubobjectDef.bTransient;
        	defaultSubobject.Property = CastFieldChecked<FObjectPropertyBase>(cls->FindPropertyByName(defaultSubobjectDef.PropertyName));
        	defaultSubobject.bRootComponent = defaultSubobjectDef.bRootComponent;
        	defaultSubobject.AttachParentDefaultSubobjectName = defaultSubobjectDef.AttachParentDefaultSubobjectName;
        	defaultSubobject.AttachSocketName = defaultSubobjectDef.AttachSocketName;
        }
        
        zscls.DefaultSubobjectOverrides.Reserve(def.DefaultSubobjectOverrides.Num());
        for (const auto& defaultObjectOverrideDef : def.DefaultSubobjectOverrides)
        {
        	FZSharpClass::FDefaultSubobjectOverride& defaultObjectOverride = zscls.DefaultSubobjectOverrides.Emplace_GetRef();
        	defaultObjectOverride.Name = defaultObjectOverrideDef.Name;
        	defaultObjectOverride.ClassPath = defaultObjectOverrideDef.ClassPath;
        }
	}
}

void ZSharp::FZUnrealFieldEmitter::EmitInterfaceSkeleton(UPackage* pak, FZInterfaceDefinition& def) const
{
	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::EmitDelegateSkeleton(UPackage* pak, FZDelegateDefinition& def) const
{
	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitStruct(UPackage* pak, FZScriptStructDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitClass(UPackage* pak, FZClassDefinition& def) const
{
	UClass* cls = def.Class;

	// Migrate from InitializePrivateStaticClass().
	// UZSharpClass must have super class.
	check(!def.SuperPath.IsNone());
	const auto superClass = FindObjectChecked<UClass>(nullptr, *def.SuperPath.ToString());
	check(superClass->IsNative());
	cls->SetSuperStruct(superClass);
	cls->ClassFlags |= superClass->ClassFlags & CLASS_ScriptInherit;

	const auto withinClass = !def.WithinPath.IsNone() ? FindObjectChecked<UClass>(nullptr, *def.WithinPath.ToString()) : UObject::StaticClass();
	check(withinClass->IsNative());
	check(withinClass->IsChildOf(UObject::StaticClass()));
	cls->ClassWithin = withinClass;

	cls->RegisterDependencies();

	// UECodeGen_Private::ConstructUClass() calls UObjectForceRegistration() to initialize statically constructed class.
	// This process sets value for ClassPrivate, OuterPrivate, etc. and add add the class to object hash.
	// StaticConstructObject_Internal() has already done these for us so we don't need to do anymore.

	cls->ClassFlags |= CLASS_Constructed;

	check(!cls->HasAnyClassFlags(CLASS_Intrinsic | CLASS_TokenStreamAssembled));
	cls->ReferenceSchema.Reset();

	// Engine code construct functions first and this can make side effects to class instance
	// so we keep sync with engine code.
	ZUnrealFieldEmitter_Private::EmitFunctions(cls, def.Functions);
	ZUnrealFieldEmitter_Private::EmitProperties(cls, def.Properties);

	// If we have no ClassConfigName then inherit from super.
	if  (cls->ClassConfigName.IsNone())
	{
		cls->ClassConfigName = superClass->ClassConfigName;
	}

	if (!def.ImplementedInterfacePaths.IsEmpty())
	{
		cls->Interfaces.Reserve(def.ImplementedInterfacePaths.Num());
		for (const auto& implementedInterfacePath : def.ImplementedInterfacePaths)
		{
			UClass* interfaceClass = FindObjectChecked<UClass>(nullptr, *implementedInterfacePath.ToString());
			check(interfaceClass->HasAllClassFlags(CLASS_Interface));
			// Interfaces implemented by UZSharpClass is always regarded as implemented in blueprint.
			FImplementedInterface implementedInterface { interfaceClass, 0, true };
			cls->Interfaces.Emplace(implementedInterface);
		}
	}

	ZUnrealFieldEmitter_Private::AddMetadata(cls, def.MetadataMap);

	cls->Bind();
	cls->StaticLink(true);
	cls->SetSparseClassDataStruct(cls->GetSparseClassDataArchetypeStruct());
	
	cls->AssembleReferenceTokenStream(true);
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitInterface(UPackage* pak, FZInterfaceDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitDelegate(UPackage* pak, FZDelegateDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::PostEmitClass(UPackage* pak, FZClassDefinition& def) const
{
	UClass* cls = def.Class;
	
	{ // Construct property defaults.
		FZSharpClass* zscls = FZSharpFieldRegistry::Get().GetMutableClass(cls);
		
		zscls->PropertyDefaults.Reserve(def.PropertyDefaults.Num());
		for (const auto& propertyDefaultDef : def.PropertyDefaults)
		{
			FZSharpClass::FPropertyDefault& propertyDefault = zscls->PropertyDefaults.Emplace_GetRef();

			// Resolve property chain.
			TArray<FString> segments;
			propertyDefaultDef.PropertyChain.ParseIntoArray(segments, TEXT("."));
			UStruct* cur = cls;
			for (int32 i = 0; i < segments.Num(); ++i)
			{
				const auto containerPropertyName = FName(segments[i]);
				FProperty* containerProperty = cur->FindPropertyByName(containerPropertyName);
				propertyDefault.PropertyChain.Emplace(containerProperty);

				if (const auto objectProp = CastField<FObjectPropertyBase>(containerProperty))
				{
					cur = objectProp->PropertyClass;
				}
				else if (const auto structProp = CastField<FStructProperty>(containerProperty))
				{
					cur = structProp->Struct;
				}
				else
				{
					check(i == segments.Num() - 1);
				}
			}
			
			propertyDefault.Buffer = propertyDefaultDef.Buffer;
		}
	}

	// Create CDO.
	(void)def.Class->GetDefaultObject();
}


