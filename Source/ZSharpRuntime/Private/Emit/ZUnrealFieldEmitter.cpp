// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealFieldEmitter.h"

#include "ZCustomThunkRegistry.h"
#include "INotifyFieldValueChanged.h"
#include "ZSharpRuntimeLogChannels.h"
#include "ZSharpFieldRegistry.h"
#include "Algo/TopologicalSort.h"
#include "ZUnrealFieldDefinitions.h"
#include "UObject/PropertyOptional.h"

#include "ZSharpClass.inl"
#include "ZSharpFunction.inl"

namespace ZSharp::ZUnrealFieldEmitter_Private
{
	static FString MakeNMZCallName(const UClass* outer, const FString& lastName)
	{
		if (lastName.IsEmpty())
		{
			return {};
		}
		
		return FString::Printf(TEXT("nm:/%s:%s"), *outer->GetPathName(), *lastName);
	}
	
	/*
	 * WARNING: This behaves differently from UHT.
	 *   - STRUCT_HasInstancedReference is shallow scan so struct won't have this flag even if it has a struct property with instanced reference. (@see: UhtScriptStruct.cs)
	 *   - CPF_ContainsInstancedReference is deep scan so struct properties whose struct has nested instanced reference also have this flag. (@see: UhtStructProperty.cs)
	 */
	static bool IsInstancedProperty(const FProperty* property)
	{
		if (property->HasAnyPropertyFlags(CPF_InstancedReference | CPF_ContainsInstancedReference))
		{
			return true;
		}
		
		auto structProperty = CastField<const FStructProperty>(property);
		return structProperty && EnumHasAllFlags(structProperty->Struct->StructFlags, STRUCT_HasInstancedReference);
	}
	
	static FString MergeNames(const FString& lhs, const FString& rhs, const FString& subtract1, const FString& subtract2)
	{
		TArray<FString> lhsNames;
		lhs.ParseIntoArray(lhsNames, TEXT(" "));

		TArray<FString> rhsNames;
		rhs.ParseIntoArray(rhsNames, TEXT(" "));

		TArray<FString> subtractNames1;
		subtract1.ParseIntoArray(subtractNames1, TEXT(" "));
		
		TArray<FString> subtractNames2;
		subtract2.ParseIntoArray(subtractNames2, TEXT(" "));

		for (const auto& name : rhsNames)
		{
			lhsNames.AddUnique(name);
		}

		for (const auto& name : subtractNames1)
		{
			lhsNames.Remove(name);
		}
		
		for (const auto& name : subtractNames2)
		{
			lhsNames.Remove(name);
		}

		return FString::Join(lhsNames, TEXT(" "));
	}
	
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
			auto owner = property->GetOwnerChecked<UField>();
			owner->AddCppProperty(property);
		}
		else
		{
			auto owner = property->GetOwnerChecked<FField>();
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
		case EZPropertyType::Bool:
			{
				NEW_PROPERTY(Bool);
				break;
			}
		case EZPropertyType::Enum:
			{
				NEW_PROPERTY(Enum);

				auto underlyingProperty = new FInt64Property { property, NAME_None, GCompiledInPropertyObjectFlags };
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
		case EZPropertyType::Utf8String:
			{
				NEW_PROPERTY(Utf8Str);
				break;
			}
		case EZPropertyType::AnsiString:
			{
				NEW_PROPERTY(AnsiStr);
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
			// @TODO: If PropertyClass has CLASS_DefaultToInstanced in hierarchy, then the property should set CPF_InstancedReference | CPF_ExportObject. (@see: UhtObjectPropertyBase.cs)
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
			/* Containers (Struct)
			 * Note: If struct has instanced reference, then the property should set CPF_ContainsInstancedReference. (@see:
			 * UhtStructProperty.cs)
			 * This could not get done here because we need globally complete reflection data.
			 * So this flag is deferred to PostEmitXXX().
			 */
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

#if DO_CHECK
		if (auto ownerStruct = owner.Get<const UStruct>())
		{
			const FProperty* existingProperty = ownerStruct->FindPropertyByName(name);
			check(!existingProperty);
		}
#endif
		
		bool needsFinishSimple = true;
		switch (def.Type)
		{
			/* Containers (except Struct)
 			 * Note: If container has struct properties with instanced reference, then the property should set CPF_ContainsInstancedReference. (@see: UhtContainerProperty.cs)
 			 * This could not get done here because we need globally complete reflection data.
 			 * So this flag is deferred to PostEmitXXX().
 			 */
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

		AddMetadata(def.Property, def.MetadataMap);

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

		UFunction* function;
		if (!def.bIsEventOverride)
		{
#if DO_CHECK
			const UFunction* existingFunction = outer->FindFunctionByName(def.Name);
			check(!existingFunction);
#endif

			FStaticConstructObjectParameters params { UFunction::StaticClass() };
			params.Outer = outer;
			params.Name = *def.Name.ToString();
			params.SetFlags = def.Flags | GCompiledInFlags;
	
			function = static_cast<UFunction*>(StaticConstructObject_Internal(params));

			function->FunctionFlags |= def.FunctionFlags;
			function->RPCId = def.RpcId;
			function->RPCResponseId = def.RpcResponseId;

			EmitProperties(function, def.Properties);
			AddMetadata(function, def.MetadataMap);
		}
		else
		{
#if DO_CHECK
			const UFunction* existingFunction = outer->FindFunctionByName(def.Name, EIncludeSuperFlag::ExcludeSuper);
			check(!existingFunction);
#endif
			
			UFunction* superFunction = outer->GetSuperClass()->FindFunctionByName(def.Name);
			check(superFunction);
			check(superFunction->HasAllFunctionFlags(FUNC_Event));

			FObjectDuplicationParameters params { superFunction, outer };
			params.DestName = *def.Name.ToString();
			params.ApplyFlags = GCompiledInFlags;

			function = static_cast<UFunction*>(StaticDuplicateObjectEx(params));
			check(function->IsSignatureCompatibleWith(superFunction));

			function->FunctionFlags |= FUNC_Native;

			// Migrate from FBlueprintCompilationManager::FastGenerateSkeletonClass().
			function->SetSuperStruct(superFunction);
			function->FunctionFlags |= (superFunction->FunctionFlags & (FUNC_FuncInherit | FUNC_Public | FUNC_Protected | FUNC_Private | FUNC_BlueprintPure));

#if WITH_METADATA
			UMetaData::CopyMetadata(superFunction, function);
#endif
		}

		def.Function = function;

		if (function->HasAllFunctionFlags(FUNC_Native))
		{
			FNativeFuncPtr thunk = def.CustomThunkName.IsNone() ? ZSharpFunction_Private::execZCall : FZCustomThunkRegistry::Get().GetThunk(def.CustomThunkName);
			outer->AddNativeFunction(*function->GetName(), thunk);
		}

		function->Bind();
		function->StaticLink(true);

		// Migrate from UClass::CreateLinkAndAddChildFunctionsToMap().
		function->Next = outer->Children;
		outer->Children = function;
		outer->AddFunctionToFunctionMap(function, function->GetFName());

		{ // Finally setup proxy.
			FZSharpFunction& zsfunction = FZSharpFieldRegistry::Get().RegisterFunction(function);
			zsfunction.ZCallName = MakeNMZCallName(outer, def.ZCallName);
			zsfunction.ValidateZCallName = MakeNMZCallName(outer, def.ValidateZCallName);
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
	static FZUnrealFieldEmitter GInstance;

	return GInstance;
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
	TArray<const UClass*> topologicallySortedClasses;
	TMap<const UClass*, FZClassDefinition*> class2def;
	{
		for (auto& def : manifest.Classes)
		{
			topologicallySortedClasses.Emplace(def.Class);
			class2def.Emplace(def.Class, &def);
		}
		
		Algo::TopologicalSort(topologicallySortedClasses, [&class2def](const UClass* cls)
		{
			const FZClassDefinition* classDef = class2def.FindChecked(cls);
			TArray<const UClass*, TInlineAllocator<2>> dependencies;
			
			if (auto superClass = FindObjectChecked<const UClass>(nullptr, *classDef->SuperPath.ToString()); FZSharpFieldRegistry::Get().IsZSharpClass(superClass))
			{
				dependencies.Emplace(superClass);
			}
			
			if (!classDef->WithinPath.IsNone())
			{
				if (auto withinClass = FindObjectChecked<const UClass>(nullptr, *classDef->WithinPath.ToString()); FZSharpFieldRegistry::Get().IsZSharpClass(withinClass))
				{
					dependencies.Emplace(withinClass);
				}
			}
			
			return dependencies;
		});
		
		for (auto cls : topologicallySortedClasses)
		{
			FZClassDefinition* def = class2def.FindChecked(cls);
			FinishEmitClass(pak, *def);
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

	// Compile class but don't create CDO.
	// Because the constructor of a CDO might trigger the creation of other CDOs,
	// we should at least guarantee that all classes have been compiled before creating any CDO.
	for (auto& cls : topologicallySortedClasses)
	{
		FZClassDefinition* def = class2def.FindChecked(cls);
		PostEmitClass_I(pak, *def);
	}

	// Create CDO and do some initialization.
	for (auto& cls : topologicallySortedClasses)
	{
		FZClassDefinition* def = class2def.FindChecked(cls);
		PostEmitClass_II(pak, *def);
	}
}

void ZSharp::FZUnrealFieldEmitter::EmitEnum(UPackage* pak, FZEnumDefinition& def) const
{
	// UEnum only has RF_Public | RF_Transient | RF_MarkAsNative set in UHT generated code,
	// but I think it's okay to keep sync with UClass.
	constexpr EObjectFlags GCompiledInFlags = RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet;
	
	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);

	FStaticConstructObjectParameters params { UEnum::StaticClass() };
	params.Outer = pak;
	params.Name = *def.Name.ToString();
	params.SetFlags = def.Flags | GCompiledInFlags;
	
	auto enm = static_cast<UEnum*>(StaticConstructObject_Internal(params));
	def.Enum = enm;

	// Migrate from UECodeGen_Private::ConstructUEnum().
	TArray<TPair<FName, int64>> EnumNames;
	EnumNames.Reserve(def.ValueMap.Num());
	for (const auto& pair : def.ValueMap)
	{
		// Migrate from UEnum::GenerateFullEnumName(), ECppForm::EnumClass case.
		EnumNames.Emplace(FString::Printf(TEXT("%s::%s"), *enm->GetName(), *pair.Key), pair.Value);
	}

	enm->SetEnums(EnumNames, UEnum::ECppForm::EnumClass, def.EnumFlags, true);
	enm->CppType = enm->GetName();
	enm->SetEnumDisplayNameFn(nullptr);

	ZUnrealFieldEmitter_Private::AddMetadata(enm, def.MetadataMap);

	// Migrate from GetStaticEnum().
	NotifyRegistrationEvent(*enm->GetOutermost()->GetName(), *enm->GetName(), ENotifyRegistrationType::NRT_Enum, ENotifyRegistrationPhase::NRP_Finished, nullptr, false, enm);
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
	
	auto cls = static_cast<UClass*>(StaticConstructObject_Internal(params));
	def.Class = cls;

	// Migrate from UClass(EStaticConstructor).
	cls->ClassConstructor = ZSharpClass_Private::ClassConstructor;
	// UZSharpClass has no ClassVTableHelperCtorCaller and CppClassStaticFunctions so they will automatically copy from super in Bind().
	cls->ClassUnique = 0;
	cls->bCooked = false;
	cls->bLayoutChanging = false;
	cls->ClassFlags = def.ClassFlags | GCompiledInClassFlags;
	cls->ClassCastFlags = def.CastFlags;
	// ClassWithin, CDO, NetFields, SparseClassData will be set later.
#if WITH_EDITORONLY_DATA
	cls->ClassGeneratedBy = nullptr; // UZSharpClass doesn't depend on blueprint.
	cls->PropertiesPendingDestruction = nullptr;
#endif

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
        	defaultSubobject.PropertyName = defaultSubobjectDef.PropertyName;
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
	auto superClass = FindObjectChecked<UClass>(nullptr, *def.SuperPath.ToString());
	check(superClass->IsNative());
	cls->SetSuperStruct(superClass);
	cls->ClassFlags |= superClass->ClassFlags & CLASS_ScriptInherit;

	auto withinClass = !def.WithinPath.IsNone() ? FindObjectChecked<UClass>(nullptr, *def.WithinPath.ToString()) : UObject::StaticClass();
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
#if WITH_METADATA
			check(!interfaceClass->HasMetaData("CannotImplementInterfaceInBlueprint"));
#endif
			// Interfaces implemented by UZSharpClass is always regarded as implemented in blueprint.
			FImplementedInterface implementedInterface { interfaceClass, 0, true };
			cls->Interfaces.Emplace(implementedInterface);
		}
	}
	
	ZUnrealFieldEmitter_Private::AddMetadata(cls, def.MetadataMap);

#if WITH_METADATA

#define MERGE_METADATA(LhsKey, SubtractKey1, SubtractKey2) \
	{ \
		const FString* pLhsName = def.TransparentDataMap.Find(#LhsKey); \
		const FString lhsName = pLhsName ? *pLhsName : FString{}; \
		const FString rhsName = cls->GetSuperClass()->GetMetaData(#LhsKey); \
		const FString* pSubtractName1 = def.TransparentDataMap.Find(#SubtractKey1); \
		const FString subtractName1 = pSubtractName1 ? *pSubtractName1 : FString{}; \
		const FString* pSubtractName2 = def.TransparentDataMap.Find(#SubtractKey2); \
		const FString subtractName2 = pSubtractName2 ? *pSubtractName2 : FString{}; \
		cls->SetMetaData(#LhsKey, *ZUnrealFieldEmitter_Private::MergeNames(lhsName, rhsName, subtractName1, subtractName2)); \
	}
	
	MERGE_METADATA(ClassGroup,,);
	MERGE_METADATA(HideFunctions, ShowFunctions,);
	MERGE_METADATA(ShowCategories,,);
	MERGE_METADATA(HideCategories,,);
	MERGE_METADATA(AutoExpandCategories,,);
	MERGE_METADATA(AutoCollapseCategories, DontAutoCollapseCategories, AutoExpandCategories);
	MERGE_METADATA(PrioritizeCategories,,);

#undef MERGE_METADATA
	
#endif
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitInterface(UPackage* pak, FZInterfaceDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitDelegate(UPackage* pak, FZDelegateDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::PostEmitClass_I(UPackage* pak, FZClassDefinition& def) const
{
	UClass* cls = def.Class;

	/* Fixup CLASS_HasInstancedReference for ourselves and CPF_InstancedReference | CPF_ContainsInstancedReference for our own properties.
	 * This must be deferred here because we need globally complete reflection data.
	 *
	 * @FIXME: Currently, instanced container properties may be wrong.
	 */
	{
		bool containsInstancedRef = false;
		for (TFieldIterator<FProperty> it(cls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
		{
			FProperty* property = *it;
			
			// First fixup CPF_InstancedReference for object/interface properties whose class has CLASS_DefaultToInstanced.
			if (auto objectProperty = CastField<const FObjectPropertyBase>(property))
			{
				if (objectProperty->PropertyClass->HasAllClassFlags(CLASS_DefaultToInstanced))
				{
					property->PropertyFlags |= CPF_InstancedReference | CPF_ExportObject;
				}
			}

			if (auto interfaceProperty = CastField<const FInterfaceProperty>(property))
			{
				if (interfaceProperty->InterfaceClass->HasAllClassFlags(CLASS_DefaultToInstanced))
				{
					property->PropertyFlags |= CPF_InstancedReference | CPF_ExportObject;
				}
			}
			
			/* Then fixup CPF_ContainsInstancedReference.
			 * There are three cases we should care about:
			 *   I: property has CPF_InstancedReference regardless of whether it's class has CLASS_HasInstancedReference.
			 *  II: property is a struct with instanced reference. (All structs should be ready to use at this point)
			 * III: property is a container with struct element with instanced reference.
			 */
			if (property->HasAllPropertyFlags(CPF_InstancedReference))
			{
				containsInstancedRef = true;
			}

			if (ZUnrealFieldEmitter_Private::IsInstancedProperty(property))
			{
				property->PropertyFlags |= CPF_ContainsInstancedReference;
				containsInstancedRef = true;
			}

			if (auto arrayProperty = CastField<FArrayProperty>(property))
			{
				if (ZUnrealFieldEmitter_Private::IsInstancedProperty(arrayProperty->Inner))
				{
					arrayProperty->Inner->PropertyFlags |= CPF_ContainsInstancedReference;
					arrayProperty->PropertyFlags |= CPF_ContainsInstancedReference;
					containsInstancedRef = true;
				}
			}

			if (auto setProperty = CastField<FSetProperty>(property))
			{
				if (ZUnrealFieldEmitter_Private::IsInstancedProperty(setProperty->ElementProp))
				{
					setProperty->ElementProp->PropertyFlags |= CPF_ContainsInstancedReference;
					setProperty->PropertyFlags |= CPF_ContainsInstancedReference;
					containsInstancedRef = true;
				}
			}

			if (auto mapProperty = CastField<FMapProperty>(property))
			{
				if (ZUnrealFieldEmitter_Private::IsInstancedProperty(mapProperty->KeyProp))
				{
					mapProperty->KeyProp->PropertyFlags |= CPF_ContainsInstancedReference;
					mapProperty->PropertyFlags |= CPF_ContainsInstancedReference;
					containsInstancedRef = true;
				}

				if (ZUnrealFieldEmitter_Private::IsInstancedProperty(mapProperty->ValueProp))
				{
					mapProperty->ValueProp->PropertyFlags |= CPF_ContainsInstancedReference;
					mapProperty->PropertyFlags |= CPF_ContainsInstancedReference;
					containsInstancedRef = true;
				}
			}

			if (auto optionalProperty = CastField<FOptionalProperty>(property))
			{
				if (ZUnrealFieldEmitter_Private::IsInstancedProperty(optionalProperty->GetValueProperty()))
				{
					optionalProperty->GetValueProperty()->PropertyFlags |= CPF_ContainsInstancedReference;
					optionalProperty->PropertyFlags |= CPF_ContainsInstancedReference;
					containsInstancedRef = true;
				}
			}
		}

		// Finally fixup CLASS_HasInstancedReference.
		if (containsInstancedRef)
		{
			cls->ClassFlags |= CLASS_HasInstancedReference;
		}
	}

	// Compile unreal class.
	cls->Bind();
	cls->StaticLink(true);
	cls->SetSparseClassDataStruct(cls->GetSparseClassDataArchetypeStruct());
	
	cls->AssembleReferenceTokenStream(true);

	// Compile Z# class.
	FZSharpClass* zscls = FZSharpFieldRegistry::Get().GetMutableClass(cls);
	// Cache DSO properties
	for (auto& defaultSubobject : zscls->DefaultSubobjects)
	{
		defaultSubobject.Property = CastFieldChecked<FObjectPropertyBase>(cls->FindPropertyByName(defaultSubobject.PropertyName));
	}
	
	{ // Construct property defaults.
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
				auto containerPropertyName = FName { segments[i] };
				FProperty* containerProperty = cur->FindPropertyByName(containerPropertyName);
				propertyDefault.PropertyChain.Emplace(containerProperty);

				if (auto objectProp = CastField<const FObjectPropertyBase>(containerProperty))
				{
					cur = objectProp->PropertyClass;
				}
				else if (auto structProp = CastField<const FStructProperty>(containerProperty))
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

	// Notify registration. Migrate from UObjectLoadAllCompiledInDefaultProperties().
	{
		TCHAR PackageName[FName::StringBufferSize];
		TCHAR ClassName[FName::StringBufferSize];
		cls->GetOutermost()->GetFName().ToString(PackageName);
		cls->GetFName().ToString(ClassName);
		NotifyRegistrationEvent(PackageName, ClassName, ENotifyRegistrationType::NRT_Class, ENotifyRegistrationPhase::NRP_Finished, nullptr, false, cls);
	}
}

void ZSharp::FZUnrealFieldEmitter::PostEmitClass_II(UPackage* pak, FZClassDefinition& def) const
{
	UClass* cls = def.Class;
	FZSharpClass* zscls = FZSharpFieldRegistry::Get().GetMutableClass(cls);
	const UClass* superCls = cls->GetSuperClass();
	
	// Create CDO.
	const UObject* cdo = def.Class->GetDefaultObject();

	// Precache replicated properties, then setup runtime replication data.
	// This must be done after create CDO. (see UClass::SetUpRuntimeReplicationData() and UClass::ValidateRuntimeReplicationData())
	{
		TMap<FName, const FZPropertyDefinition*> netPropertyName2Def;
		for (const auto& propertyDef : def.Properties)
		{
			if (propertyDef.PropertyFlags & CPF_Net)
			{
				netPropertyName2Def.Emplace(propertyDef.Name, &propertyDef);
			}
		}
		
		// Super class's ClassReps should be ready to use at this point.
		auto currentRepIndex = static_cast<uint16>(superCls->ClassReps.Num());
		for (TFieldIterator<FProperty> it(cls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
		{
			FProperty* property = *it;
			if (property->HasAllPropertyFlags(CPF_Net))
			{
				const FZPropertyDefinition** pPropertyDef = netPropertyName2Def.Find(property->GetFName());
				check(pPropertyDef && *pPropertyDef);
				const FZPropertyDefinition& propertyDef = **pPropertyDef;
				check(property->RepIndex == 0);
				property->RepIndex = currentRepIndex++;
				zscls->ReplicatedProperties.Emplace(FZSharpClass::FReplicatedProperty { property, propertyDef.RepCondition, propertyDef.RepNotifyCondition, propertyDef.IsRepPushBased });
			}
		}

		cls->SetUpRuntimeReplicationData();
		
#if DO_CHECK
		// Migrate from UClass::ValidateRuntimeReplicationData().
		TArray<FLifetimeProperty> lifetimeProps;
		lifetimeProps.Reserve(cls->ClassReps.Num());
		cdo->GetLifetimeReplicatedProps(lifetimeProps);

		checkf(lifetimeProps.Num() == cls->ClassReps.Num(), TEXT("Some replicated properties don't get registered."));
#endif
	}

	// Construct field notifies.
	if (!def.FieldNotifies.IsEmpty())
	{
		const UObject* superCdo = superCls->GetDefaultObject(false);
		check(superCdo);
		
		const INotifyFieldValueChanged* superInterface = Cast<INotifyFieldValueChanged>(superCdo);
		int32 currentFieldNotifyIndex;
		if (superInterface)
		{
			superInterface->GetFieldNotificationDescriptor().ForEachField(superCls, [&currentFieldNotifyIndex](UE::FieldNotification::FFieldId)
			{
				++currentFieldNotifyIndex;
				return true;
			});
		}
		else
		{
			check(cls->ImplementsInterface(UNotifyFieldValueChanged::StaticClass()));
			
			currentFieldNotifyIndex = 0;
		}

		for (const auto& field : def.FieldNotifies)
		{
			zscls->FieldNotifies.Emplace(UE::FieldNotification::FFieldId { field, currentFieldNotifyIndex++ });
		}
	}

	UE_LOG(LogZSharpEmit, Log, TEXT("Successfully emit Z# class [%s]!"), *cls->GetPathName());
}


