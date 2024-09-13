// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealFieldEmitter.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ZSharpFieldRegistry.h"
#include "Algo/TopologicalSort.h"
#include "ZUnrealFieldDefinitions.h"
#include "UObject/PropertyOptional.h"

namespace ZSharp::ZSharpClass_Private
{
	static void ConstructObject(const FObjectInitializer& objectInitializer, UClass* cls)
	{
		UObject* obj = objectInitializer.GetObj();

		const bool zscls = FZSharpFieldRegistry::Get().IsZSharpClass(cls);
		const bool native = cls->IsNative();
		// If we are dynamic class (Z# class or BPGC) then call super constructor first.
		if (zscls || !native)
		{
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
			for (TFieldIterator<FProperty> it(cls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
			{
				// @TODO: Default value.
				it->InitializeValue_InContainer(obj);
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

	static void FinishEmitProperty(FZSimplePropertyDefinition& def)
	{
		FProperty* property = def.Property;
		property->PropertyFlags |= def.PropertyFlags;
		property->RepNotifyFunc = def.RepNotifyName;

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

	static constexpr EObjectFlags GCompiledInPropertyObjectFlags = RF_Public | RF_Transient;
	static constexpr EPropertyFlags PrimitiveFlags = CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash;

#define NEW_PROPERTY(PropertyTypeName, CompiledInPropertyFlags) \
	auto property = new F##PropertyTypeName##Property(owner, name, flags | GCompiledInPropertyObjectFlags); \
	property->PropertyFlags |= CompiledInPropertyFlags; \
	def.Property = property;
	
	static void EmitSimpleProperty(FFieldVariant owner, FZSimplePropertyDefinition& def, FName name, EObjectFlags flags)
	{
		switch (def.Type)
		{
			// Primitives
		case EZPropertyType::UInt8:
			{
				NEW_PROPERTY(Byte, PrimitiveFlags);
				break;
			}
		case EZPropertyType::UInt16:
			{
				NEW_PROPERTY(UInt16, PrimitiveFlags);
				break;
			}
		case EZPropertyType::UInt32:
			{
				NEW_PROPERTY(UInt32, PrimitiveFlags);
				break;
			}
		case EZPropertyType::UInt64:
			{
				NEW_PROPERTY(UInt64, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Int8:
			{
				NEW_PROPERTY(Int8, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Int16:
			{
				NEW_PROPERTY(Int16, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Int32:
			{
				NEW_PROPERTY(Int, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Int64:
			{
				NEW_PROPERTY(Int64, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Float:
			{
				NEW_PROPERTY(Float, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Double:
			{
				NEW_PROPERTY(Double, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Enum:
			{
				NEW_PROPERTY(Enum, PrimitiveFlags);

				const auto underlyingProperty = new FInt64Property(property, NAME_None, GCompiledInPropertyObjectFlags);
				property->ElementSize = underlyingProperty->ElementSize;
				property->SetEnum(FindObjectChecked<UEnum>(nullptr, *def.DescriptorFieldPath.ToString()));
				property->AddCppProperty(underlyingProperty);
				
				break;
			}
			// Strings
		case EZPropertyType::String:
			{
				NEW_PROPERTY(Str, CPF_ZeroConstructor | CPF_HasGetValueTypeHash);
				break;
			}
		case EZPropertyType::Name:
			{
				NEW_PROPERTY(Name, PrimitiveFlags);
				break;
			}
		case EZPropertyType::Text:
			{
				NEW_PROPERTY(Text, CPF_None);
				break;
			}
			// Object wrappers
		case EZPropertyType::Object:
			{
				NEW_PROPERTY(Object, PrimitiveFlags | CPF_TObjectPtrWrapper);

				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::Class:
			{
				NEW_PROPERTY(Class, CPF_UObjectWrapper | CPF_HasGetValueTypeHash);

				property->PropertyClass = UClass::StaticClass();
				property->MetaClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::SoftClass:
			{
				NEW_PROPERTY(SoftClass, CPF_UObjectWrapper | CPF_HasGetValueTypeHash);

				property->MetaClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::SoftObject:
			{
				NEW_PROPERTY(SoftObject, CPF_UObjectWrapper | CPF_HasGetValueTypeHash);

				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::WeakObject:
			{
				NEW_PROPERTY(WeakObject, PrimitiveFlags | CPF_UObjectWrapper);

				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::LazyObject:
			{
				NEW_PROPERTY(LazyObject, CPF_IsPlainOldData | CPF_NoDestructor | CPF_UObjectWrapper | CPF_HasGetValueTypeHash);

				property->PropertyClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::Interface:
			{
				NEW_PROPERTY(Interface, PrimitiveFlags | CPF_UObjectWrapper);

				property->InterfaceClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
			// Containers (Struct)
		case EZPropertyType::Struct:
			{
				NEW_PROPERTY(Struct, CPF_None);

				property->Struct = FindObjectChecked<UScriptStruct>(nullptr, *def.DescriptorFieldPath.ToString());
				property->ElementSize = property->Struct->GetStructureSize();
				
				break;
			}
			// Delegates
		case EZPropertyType::Delegate:
			{
				NEW_PROPERTY(Delegate, CPF_None);

				property->SignatureFunction = FindObjectChecked<UDelegateFunction>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::MulticastInlineDelegate:
			{
				NEW_PROPERTY(MulticastInlineDelegate, CPF_None);
				
				property->SignatureFunction = FindObjectChecked<UDelegateFunction>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
			// Special types
		case EZPropertyType::FieldPath:
			{
				NEW_PROPERTY(FieldPath, CPF_HasGetValueTypeHash);

				property->PropertyClass = FProperty::StaticClass();
				
				break;
			}
		default:
			{
				UE_LOG(LogZSharpEmit, Fatal, TEXT("Unknown property type [%d]!!!"), def.Type);
				break;
			}
		}

		FinishEmitProperty(def);
	}

	static void EmitProperty(FFieldVariant owner, FZPropertyDefinition& def)
	{
		const FName name = def.Name;
		const EObjectFlags flags = def.Flags;
		
		bool needsFinish = true;
		switch (def.Type)
		{
			// Containers (except Struct)
		case EZPropertyType::Array:
			{
				NEW_PROPERTY(Array, CPF_None);

				EmitSimpleProperty(property, def.InnerProperty, {}, RF_NoFlags);

				break;
			}
		case EZPropertyType::Set:
			{
				NEW_PROPERTY(Set, CPF_None);

				EmitSimpleProperty(property, def.InnerProperty, {}, RF_NoFlags);

				break;
			}
		case EZPropertyType::Map:
			{
				NEW_PROPERTY(Map, CPF_None);

				EmitSimpleProperty(property, def.InnerProperty, {}, RF_NoFlags);
				EmitSimpleProperty(property, def.OuterProperty, {}, RF_NoFlags);

				break;
			}
		case EZPropertyType::Optional:
			{
				NEW_PROPERTY(Optional, CPF_None);

				EmitSimpleProperty(property, def.InnerProperty, {}, RF_NoFlags);

				break;
			}
		default:
			{
				EmitSimpleProperty(owner, def, name, flags);
				needsFinish = false;
				break;
			}
		}

		if (needsFinish)
		{
			FinishEmitProperty(def);
		}
	}

#undef NEW_PROPERTY

	static void EmitProperties(UStruct* outer, TMap<FName, FZPropertyDefinition>& def)
	{
		for (auto& pair : def)
		{
			EmitProperty(outer, pair.Value);
		}
	}

	static void EmitFunction(UClass* outer, FZFunctionDefinition& def)
	{
		// Migrate from UECodeGen_Private::ConstructUFunction().
		constexpr EObjectFlags GCompiledInFlags = RF_Public | RF_Transient | RF_MarkAsNative;

		FStaticConstructObjectParameters params { UFunction::StaticClass() };
		params.Outer = outer;
		params.Name = *def.Name.ToString();
		params.SetFlags = def.Flags | GCompiledInFlags;
	
		const auto function = static_cast<UFunction*>(StaticConstructObject_Internal(params));
		FZSharpFieldRegistry::Get().RegisterFunction(function);
		def.Function = function;

		function->FunctionFlags |= def.FunctionFlags;
		function->RPCId = def.RpcId;
		function->RPCResponseId = def.RpcResponseId;

		EmitProperties(function, def.PropertyMap);
		AddMetadata(function, def.MetadataMap);

		function->Bind();
		function->StaticLink(true);

		// Migrate from UClass::CreateLinkAndAddChildFunctionsToMap().
		function->Next = outer->Children;
		outer->Children = function;
		outer->AddFunctionToFunctionMap(function, function->GetFName());
	}

	static void EmitFunctions(UClass* outer, TMap<FName, FZFunctionDefinition>& def)
	{
		for (auto& pair : def)
		{
			EmitFunction(outer, pair.Value);
		}
	}
}

ZSharp::FZUnrealFieldEmitter& ZSharp::FZUnrealFieldEmitter::Get()
{
	static FZUnrealFieldEmitter GSingleton;

	return GSingleton;
}

void ZSharp::FZUnrealFieldEmitter::Emit(FZUnrealFieldManifest& def)
{
	UE_CLOG(bEmitting, LogZSharpEmit, Fatal, TEXT("Emit is a package-wide atomic operation so can't invoke recursively!!!"));
	TGuardValue recursionGuard { bEmitting, true };

	if (EmittedModules.Contains(def.ModuleName))
	{
		return;
	}
	EmittedModules.Emplace(def.ModuleName);
	
	InternalEmit(def);
}

void ZSharp::FZUnrealFieldEmitter::InternalEmit(FZUnrealFieldManifest& def) const
{
	if (def.EnumMap.IsEmpty() && def.StructMap.IsEmpty() && def.ClassMap.IsEmpty() && def.InterfaceMap.IsEmpty() && def.DelegateMap.IsEmpty())
	{
		return;
	}
	
	// Migrate from UObjectBase::DeferredRegister().
	const FString packageName = FString::Printf(TEXT("/Script/%s"), *def.ModuleName.ToString());
	UPackage* pak = CreatePackage(*packageName);
	def.Package = pak;

	pak->SetPackageFlags(PKG_CompiledIn);

	// Enums have no dependency so we can emit them just one step.
	for (auto& pair : def.EnumMap)
	{
		EmitEnum(pak, pair.Value);
	}
	
	// Other fields have dependency so we emit skeleton (placeholder) first.
	for (auto& pair : def.StructMap)
	{
		EmitStructSkeleton(pak, pair.Value);
	}

	for (auto& pair : def.ClassMap)
	{
		EmitClassSkeleton(pak, pair.Value);
	}

	for (auto& pair : def.InterfaceMap)
	{
		EmitInterfaceSkeleton(pak, pair.Value);
	}

	for (auto& pair : def.DelegateMap)
	{
		EmitDelegateSkeleton(pak, pair.Value);
	}

	// Now that all fields are in memory (but not fully initialized) and we can setup dependency.
	// IMPORTANT: Structs must finish before others because properties depend on structs need to calculate size.
	// @TODO: Sort structs by reference.
	for (auto& pair : def.StructMap)
	{
		FinishEmitStruct(pak, pair.Value);
	}

	// Subclass may depend on super/within class's data i.e. ClassConfigName, SparseClassDataStruct.
	// So we sort classes to ensure super/within class get constructed before subclass.
	// Algo::TopologicalSort doesn't support projection or custom hash, so we manually make a projection.
	{
		TArray<const UClass*> classes;
		TMap<const UClass*, FZClassDefinition*> map;
		for (auto& pair : def.ClassMap)
		{
			classes.Emplace(pair.Value.Class);
			map.Emplace(pair.Value.Class, &pair.Value);
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

	for (auto& pair : def.InterfaceMap)
	{
		FinishEmitInterface(pak, pair.Value);
	}

	for (auto& pair : def.DelegateMap)
	{
		FinishEmitDelegate(pak, pair.Value);
	}

	// Finally, create CDO for all classes.
	for (const auto& pair : def.ClassMap)
	{
		(void)pair.Value.Class->GetDefaultObject();
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
	FZSharpFieldRegistry::Get().RegisterClass(cls);
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
	cls->ClassFlags |= superClass->ClassFlags & CLASS_Inherit;

	const auto withinClass = !def.WithinPath.IsNone() ? FindObjectChecked<UClass>(nullptr, *def.WithinPath.ToString()) : UObject::StaticClass();
	check(withinClass->IsNative());
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
	ZUnrealFieldEmitter_Private::EmitFunctions(cls, def.FunctionMap);
	ZUnrealFieldEmitter_Private::EmitProperties(cls, def.PropertyMap);

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


