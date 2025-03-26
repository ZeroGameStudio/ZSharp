// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealFieldEmitter.h"

#include "ZCustomThunkRegistry.h"
#include "INotifyFieldValueChanged.h"
#include "ZSharpRuntimeLogChannels.h"
#include "ZSharpFieldRegistry.h"
#include "Algo/TopologicalSort.h"
#include "ZUnrealFieldDefinitions.h"
#include "UObject/PropertyOptional.h"

#include "ZSharpScriptStruct.inl"
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
	
	static bool IsInstancedProperty(const FProperty* property)
	{
		if (property->HasAnyPropertyFlags(CPF_InstancedReference | CPF_ContainsInstancedReference))
		{
			return true;
		}
		
		auto structProperty = CastField<const FStructProperty>(property);
		return structProperty && EnumHasAllFlags(structProperty->Struct->StructFlags, STRUCT_HasInstancedReference);
	}

	// Fixup implicit instanced flags caused by CLASS_DefaultToInstanced.
	static bool FixupInstancedFlagsForObjectProperty(FProperty* property)
	{
		// Migrate from UHT. (@see: UhtObjectPropertyBase.cs and UhtInterfaceProperty.cs)
		static constexpr EPropertyFlags GDefaultToInstancedPropertyFlags = CPF_InstancedReference | CPF_ExportObject; // No CPF_PersistentObject.
		static constexpr EPropertyFlags GDefaultToInstancedPropertyFlagsForInputParameter = CPF_InstancedReference; // Input parameter can't have CPF_ExportObject.
		
		if (auto objectProperty = CastField<const FObjectPropertyBase>(property))
		{
			if (objectProperty->PropertyClass->HasAllClassFlags(CLASS_DefaultToInstanced))
			{
				FProperty* ownerProperty = property->GetOwnerProperty();
				property->PropertyFlags |= ownerProperty->HasAllPropertyFlags(CPF_Parm) && !ownerProperty->HasAnyPropertyFlags(CPF_OutParm) ? GDefaultToInstancedPropertyFlagsForInputParameter : GDefaultToInstancedPropertyFlags;
				ownerProperty->SetMetaData("EditInline", "true"); // Only object property need this.
				return true;
			}
		}
		else if (auto interfaceProperty = CastField<const FInterfaceProperty>(property))
		{
			if (interfaceProperty->InterfaceClass->HasAllClassFlags(CLASS_DefaultToInstanced))
			{
				FProperty* ownerProperty = property->GetOwnerProperty();
				property->PropertyFlags |= ownerProperty->HasAllPropertyFlags(CPF_Parm) && !ownerProperty->HasAnyPropertyFlags(CPF_OutParm) ? GDefaultToInstancedPropertyFlagsForInputParameter : GDefaultToInstancedPropertyFlags;
				return true;
			}
		}

		return false;
	}

	// Fixup implicit instanced flags caused by CLASS_DefaultToInstanced and propagated instanced flags.
	static bool FixupInstancedFlagsForContainerProperty(FProperty* containerProperty, FProperty* elementProperty)
	{
		FixupInstancedFlagsForObjectProperty(elementProperty);
		
		if (IsInstancedProperty(elementProperty))
		{
			containerProperty->PropertyFlags |= CPF_ContainsInstancedReference | elementProperty->PropertyFlags & CPF_ExportObject;

			bool structProperty = containerProperty == elementProperty;
			check(structProperty == containerProperty->IsA<FStructProperty>());
			if (!structProperty)
			{
				containerProperty->PropertyFlags &= ~(CPF_InstancedReference | CPF_PersistentInstance);
				
				if (EnumHasAllFlags(static_cast<EClassCastFlags>(elementProperty->GetCastFlags()), CASTCLASS_FStructProperty))
				{
					elementProperty->PropertyFlags |= CPF_ContainsInstancedReference;
				}

#if WITH_METADATA
				if (elementProperty->HasAllPropertyFlags(CPF_PersistentInstance))
				{
					if (const TMap<FName, FString>* metadata = containerProperty->GetMetaDataMap())
					{
						for (const auto& [key, value] : *metadata)
						{
							elementProperty->SetMetaData(key, *value);
						}
					}
				}
#endif
			}

			return true;
		}

		return false;
	}

	static bool FixupInstancedFlagsForStruct(UStruct* strct)
	{
		bool containsInstancedRef = false;
		for (TFieldIterator<FProperty> it(strct, EFieldIteratorFlags::ExcludeSuper); it; ++it)
		{
			FProperty* property = *it;
			
			// Migrate from UHT. (@see: UhtObjectPropertyBase.cs, UhtInterfaceProperty.cs, UhtStructProperty.cs and UhtContainerBaseProperty.cs)
			if (EnumHasAnyFlags(static_cast<EClassCastFlags>(property->GetCastFlags()), CASTCLASS_FObjectPropertyBase | CASTCLASS_FInterfaceProperty))
			{
				containsInstancedRef |= FixupInstancedFlagsForObjectProperty(property);
			}
			else if (auto structProperty = CastField<FStructProperty>(property))
			{
				containsInstancedRef |= FixupInstancedFlagsForContainerProperty(structProperty, structProperty);
			}
			else if (auto arrayProperty = CastField<FArrayProperty>(property))
			{
				containsInstancedRef |= FixupInstancedFlagsForContainerProperty(arrayProperty, arrayProperty->Inner);
			}
			else if (auto setProperty = CastField<FSetProperty>(property))
			{
				containsInstancedRef |= FixupInstancedFlagsForContainerProperty(setProperty, setProperty->ElementProp);
			}
			else if (auto mapProperty = CastField<FMapProperty>(property))
			{
				bool fixed = FixupInstancedFlagsForContainerProperty(mapProperty, mapProperty->ValueProp);
				containsInstancedRef |= fixed;
				if (fixed)
				{
					// Propagate fixed flags from value to key.
					mapProperty->KeyProp->PropertyFlags |= mapProperty->ValueProp->PropertyFlags & (CPF_ContainsInstancedReference | CPF_ExportObject | CPF_InstancedReference);
				}
			}
			else if (auto optionalProperty = CastField<FOptionalProperty>(property))
			{
				containsInstancedRef |= FixupInstancedFlagsForContainerProperty(optionalProperty, optionalProperty->GetValueProperty());
			}
		}

		return containsInstancedRef;
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
		UMetaData* target = field->GetOutermost()->GetMetaData();
		if (!metadata.IsEmpty())
		{
			for (const auto& pair : metadata)
			{
				target->SetValue(field, pair.Key, *pair.Value);
			}
		}
		target->SetValue(field, "ZSharpEmittedField", TEXT("true"));
#endif
	}

	static void AddMetadata(FField* field, const TMap<FName, FString>& metadata)
	{
#if WITH_METADATA
		for (const auto& pair : metadata)
		{
			field->SetMetaData(pair.Key, *pair.Value);
		}
		field->SetMetaData("ZSharpEmittedField", TEXT("true"));
#endif
	}

	static void FinishEmitSimpleProperty(FZSimplePropertyDefinition& def)
	{
		FProperty* property = def.Property;
		
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
	def.Property = property; \
	property->PropertyFlags |= def.PropertyFlags;
	
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

				// Migrate from FBoolProperty constructor.
				// Emitted bool property is always treated as native bool.
				property->SetBoolSize(sizeof(bool), true);
				
				break;
			}
		case EZPropertyType::Enum:
			{
				static const FName GUnderlyingPropertyName { "UnderlyingType" };
				
				NEW_PROPERTY(Enum);

				property->SetEnum(FindObjectChecked<UEnum>(nullptr, *def.DescriptorFieldPath.ToString()));

				FProperty* underlyingProperty = nullptr;
				switch (def.EnumUnderlyingType)
				{
				case EZEnumUnderlyingType::UInt8:
					{
						underlyingProperty = new FByteProperty { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}
				case EZEnumUnderlyingType::UInt16:
					{
						underlyingProperty = new FUInt16Property { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}
				case EZEnumUnderlyingType::UInt32:
					{
						underlyingProperty = new FUInt32Property { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}
				case EZEnumUnderlyingType::UInt64:
					{
						underlyingProperty = new FUInt64Property { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}
				case EZEnumUnderlyingType::Int8:
					{
						underlyingProperty = new FInt8Property { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}

				case EZEnumUnderlyingType::Int16:
					{
						underlyingProperty = new FInt16Property { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}
				case EZEnumUnderlyingType::Int32:
					{
						underlyingProperty = new FIntProperty { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}
				case EZEnumUnderlyingType::Int64:
					{
						underlyingProperty = new FInt64Property { property, GUnderlyingPropertyName, GCompiledInPropertyObjectFlags };
						break;
					}
				default:
					{
						break;
					}
				}
				UE_CLOG(!underlyingProperty, LogZSharpEmit, Fatal, TEXT("Enum property with unknown underlying type!!! Property: %s.%s"), *owner.GetName(), *name.ToString());
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
		case EZPropertyType::Object:
			{
				FObjectProperty* objectProperty;
				auto descriptor = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				if (descriptor->IsChildOf<UClass>())
				{
					// Special case for TObjectPtr<UClass>.
					NEW_PROPERTY(Class);

					property->MetaClass = UObject::StaticClass();
					
					objectProperty = property;
				}
				else
				{
					NEW_PROPERTY(Object);
					objectProperty = property;
				}

				// TObjectPtr<T> parameter is decayed to T*.
				if (!objectProperty->GetOwnerProperty()->HasAnyPropertyFlags(CPF_Parm))
				{
					objectProperty->PropertyFlags |= CPF_TObjectPtrWrapper; // Migrate from UHT.
				}
				
				objectProperty->PropertyClass = descriptor;
				
				break;
			}
		case EZPropertyType::Class:
			{
				NEW_PROPERTY(Class);

				property->PropertyFlags |= CPF_UObjectWrapper; // Migrate from UHT.
				
				property->PropertyClass = UClass::StaticClass();
				property->MetaClass = FindObjectChecked<UClass>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
		case EZPropertyType::SoftClass:
			{
				NEW_PROPERTY(SoftClass);

				property->PropertyFlags |= CPF_UObjectWrapper; // Migrate from UHT.

				property->PropertyClass = UClass::StaticClass();
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
			// There seems to be a TAutoWeakObjectPtr<> and CPF_AutoWeak, but I don't see...
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
			 * Note: If struct has instanced reference, then the property should set CPF_ContainsInstancedReference. (@see: UhtStructProperty.cs)
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
		case EZPropertyType::MulticastSparseDelegate:
			{
				UE_CLOG(!owner.IsA<UClass>(), LogZSharpEmit, Fatal, TEXT("Sparse delegate property must be in class!!! Property: %s.%s"), *owner.GetName(), *name.ToString());
				
				NEW_PROPERTY(MulticastSparseDelegate);

				property->PropertyFlags |= CPF_InstancedReference; // Migrate from UHT.
				
				property->SignatureFunction = FindObjectChecked<USparseDelegateFunction>(nullptr, *def.DescriptorFieldPath.ToString());
				
				break;
			}
			// Special types
		case EZPropertyType::FieldPath:
			{
				UE_LOG(LogZSharpEmit, Fatal, TEXT("FieldPath property is not supported by emit!!!"));
				
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

				// FArrayProperty doesn't call TTypeFundamentals::GetComputedFlagsPropertyFlags() to calculate computed flags,
				// we directly bake the result here (using FScriptArray).
				property->PropertyFlags |= CPF_ZeroConstructor;

				EmitSimpleProperty(property, def.InnerProperty, name, RF_NoFlags);

				// Migrate from UHT.
				property->PropertyFlags |= property->Inner->PropertyFlags & CPF_TObjectPtrWrapper;
				property->Inner->PropertyFlags = property->PropertyFlags & (CPF_PropagateToArrayInner | CPF_TObjectPtr); // Fix C++ and UHT mismatch.

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
				property->ElementProp->PropertyFlags = property->PropertyFlags & (CPF_PropagateToSetElement | CPF_TObjectPtr); // Fix C++ and UHT mismatch.

				break;
			}
		case EZPropertyType::Map:
			{
				NEW_PROPERTY(Map);

				// Assume inner property is hashable.
				def.InnerProperty.PropertyFlags |= CPF_HasGetValueTypeHash;

				EmitSimpleProperty(property, def.InnerProperty, FName(FString::Printf(TEXT("%s_Key"), *name.ToString())), RF_NoFlags);
				EmitSimpleProperty(property, def.OuterProperty, name, RF_NoFlags);

				/*
				 * Migrate from UHT.
				 * Rule:
				 * 1. Key doesn't propagate to Outer.
				 * 2. Value propagates CPF_TObjectPtrWrapper to Outer.
				 */
				property->PropertyFlags |= property->ValueProp->PropertyFlags & CPF_TObjectPtrWrapper;
				property->KeyProp->PropertyFlags = property->PropertyFlags & (CPF_PropagateToMapKey & ~CPF_UObjectWrapper | CPF_TObjectPtr) | property->KeyProp->PropertyFlags & CPF_TObjectPtrWrapper; // Fix C++ and UHT mismatch & Key only maintains CPF_UObjectWrapper on itself and CPF_TObjectPtr on both itself and outer.
				property->ValueProp->PropertyFlags = property->PropertyFlags & (CPF_PropagateToMapValue | CPF_TObjectPtr); // Fix C++ and UHT mismatch.
				
				break;
			}
		case EZPropertyType::Optional:
			{
				NEW_PROPERTY(Optional);

				EmitSimpleProperty(property, def.InnerProperty, name, RF_NoFlags);

				// Migrate from UHT.
				FProperty* valueProperty = property->GetValueProperty();
				property->PropertyFlags |= valueProperty->PropertyFlags & CPF_UObjectWrapper; // No CPF_TObjectPtr.
				valueProperty->PropertyFlags = property->PropertyFlags & CPF_PropagateToOptionalInner | valueProperty->PropertyFlags & CPF_TObjectPtr; // Value only maintains CPF_TObjectPtr on itself.

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
		// Emit by declared order. (reversed because EmitProperty() uses head-insert)
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

			function->FunctionFlags |= FUNC_Native;
			function->SetSuperStruct(superFunction);

#if WITH_METADATA
			UMetaData::CopyMetadata(superFunction, function);
			function->SetMetaData("BlueprintInternalUseOnly", TEXT("true"));
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
		function->MinAlignment = 1; // Function is always aligned by 1.

		// Now that all parameters have linked up so we can check for signature compability.
		check(!def.bIsEventOverride || function->IsSignatureCompatibleWith(function->GetSuperFunction()));

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
		// Scanner should have sorted functions so we use the list directly.
		for (auto& def : defs)
		{
			EmitFunction(outer, def);
		}
	}

	static void ConstructPropertyDefaults(FZStructDefinition& def, UStruct* strct, FZSharpStruct* zsstrct)
	{
		bool allowSubobject = strct->IsA<UClass>();
		
		zsstrct->PropertyDefaults.Reserve(def.PropertyDefaults.Num());
		for (const auto& propertyDefaultDef : def.PropertyDefaults)
		{
			FZSharpClass::FPropertyDefault& propertyDefault = zsstrct->PropertyDefaults.Emplace_GetRef();

			// Resolve property chain.
			TArray<FString> segments;
			propertyDefaultDef.PropertyChain.ParseIntoArray(segments, TEXT("."));
			UStruct* cur = strct;
			for (int32 i = 0; i < segments.Num(); ++i)
			{
				// UStruct can't set inner property value of an object property through property default because struct can't own an object.
				if (!allowSubobject && cur->IsA<UClass>())
				{
					checkNoEntry();
				}
				
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

	{ // Stage I: Emit enums because they are not struct, which means they have no dependency.
		for (auto& def : manifest.Enums)
		{
			EmitEnum(pak, def);
		}
	}

	{ // Stage II: Emit skeleton for all structs (Allocated but not get dependencies set up).
		for (auto& def : manifest.Structs)
		{
			EmitStructSkeleton(pak, def);
		}
		
		for (auto& def : manifest.Interfaces)
		{
			EmitInterfaceSkeleton(pak, def);
		}

		for (auto& def : manifest.Classes)
		{
			EmitClassSkeleton(pak, def);
		}

		// Emit delegate skeleton after class because it may be nested in class.
		for (auto& def : manifest.Delegates)
		{
			EmitDelegateSkeleton(pak, def);
		}
	}

	// Now that all fields are in memory (but not fully initialized) and we can set up dependency.

	TArray<const UScriptStruct*> topologicallySortedScriptStructs;
	TMap<const UScriptStruct*, FZScriptStructDefinition*> scriptStruct2def;
	{ // Stage III: Set up dependencies for script structs first because we need it's size almost all the time.
		// Script structs may depend on each other (reference or inherit),
		// but fortunately there is no loop because script struct acts as a value type,
		// so we sort script structs to ensure referenced structs get constructed before referencing structs.
		for (auto& def : manifest.Structs)
		{
			topologicallySortedScriptStructs.Emplace(def.ScriptStruct);
			scriptStruct2def.Emplace(def.ScriptStruct, &def);
		}
		
		bool suc = Algo::TopologicalSort(topologicallySortedScriptStructs, [&scriptStruct2def](const UScriptStruct* scriptStruct)
		{
			const FZScriptStructDefinition* scriptStructDef = scriptStruct2def.FindChecked(scriptStruct);
			TArray<const UScriptStruct*> dependencies;

			if (!scriptStructDef->SuperPath.IsNone())
			{
				if (auto superStruct = FindObjectChecked<const UScriptStruct>(nullptr, *scriptStructDef->SuperPath.ToString()); scriptStruct2def.Contains(superStruct))
				{
					dependencies.Emplace(superStruct);
				}
			}

			auto addDependency = [&scriptStruct2def, &dependencies](FName dependencyFieldPath)
			{
				if (auto referencedStruct = FindObjectChecked<const UScriptStruct>(nullptr, *dependencyFieldPath.ToString()); scriptStruct2def.Contains(referencedStruct))
				{
					dependencies.Emplace(referencedStruct);
				}
			};

			for (const auto& propertyDef : scriptStructDef->Properties)
			{
				// More strict than the engine - any kind of in-module circular struct reference is not allowed, not only directly reference.
				switch (propertyDef.Type)
				{
				case EZPropertyType::Struct:
					{
						addDependency(propertyDef.DescriptorFieldPath);
						break;
					}
				case EZPropertyType::Array:
				case EZPropertyType::Set:
				case EZPropertyType::Map:
				case EZPropertyType::Optional:
					{
						if (propertyDef.InnerProperty.Type == EZPropertyType::Struct)
						{
							addDependency(propertyDef.InnerProperty.DescriptorFieldPath);
						}

						if (propertyDef.Type == EZPropertyType::Map && propertyDef.OuterProperty.Type == EZPropertyType::Struct)
						{
							addDependency(propertyDef.OuterProperty.DescriptorFieldPath);
						}
						
						break;
					}
				default:
					{
						break;
					}
				}
			}
		
			return dependencies;
		});

		if (!suc)
		{
			UE_LOG(LogZSharpEmit, Fatal, TEXT("Cycle detected in script struct dependency graph!!!"));
		}

		for (auto scriptStruct : topologicallySortedScriptStructs)
		{
			FZScriptStructDefinition* scriptStructDef = scriptStruct2def.FindChecked(scriptStruct);
			FinishEmitStruct(pak, *scriptStructDef);
		}
	}

	// Now that all value types have known their size thus we can link up everything include reference types.

	{ // Stage IV: Set up dependencies for delegates.
		// For those delegates nested in UClass, we only use UClass object as outer so skeleton is enough.
		for (auto& def : manifest.Delegates)
		{
			FinishEmitDelegate(pak, def);
		}
	}

	{ // Stage V: Set up dependencies for interfaces before classes because classes can implement interfaces.
		for (auto& def : manifest.Interfaces)
		{
			FinishEmitInterface(pak, def);
		}
	}

	TArray<const UClass*> topologicallySortedClasses;
	TMap<const UClass*, FZClassDefinition*> class2def;
	{ // Stage VI: Set up dependencies for classes.
		// Subclass may depend on super/within class's data i.e. ClassConfigName, SparseClassDataStruct,
		// so we sort classes to ensure super/within classes get constructed before subclasses.
		for (auto& def : manifest.Classes)
		{
			topologicallySortedClasses.Emplace(def.Class);
			class2def.Emplace(def.Class, &def);
		}
		
		bool suc = Algo::TopologicalSort(topologicallySortedClasses, [&class2def](const UClass* cls)
		{
			const FZClassDefinition* classDef = class2def.FindChecked(cls);
			TArray<const UClass*, TInlineAllocator<2>> dependencies;
			
			if (auto superClass = FindObjectChecked<const UClass>(nullptr, *classDef->SuperPath.ToString()); class2def.Contains(superClass))
			{
				dependencies.Emplace(superClass);
			}
			
			if (!classDef->WithinPath.IsNone())
			{
				if (auto withinClass = FindObjectChecked<const UClass>(nullptr, *classDef->WithinPath.ToString()); class2def.Contains(withinClass))
				{
					dependencies.Emplace(withinClass);
				}
			}
			
			return dependencies;
		});
		
		if (!suc)
		{
			UE_LOG(LogZSharpEmit, Fatal, TEXT("Cycle detected in class dependency graph!!!"));
		}
		
		for (auto cls : topologicallySortedClasses)
		{
			FZClassDefinition* def = class2def.FindChecked(cls);
			FinishEmitClass(pak, *def);
		}
	}

	{ // Stage VII: Finalize compilation.
		for (auto& scriptStruct : topologicallySortedScriptStructs)
		{
			FZScriptStructDefinition* scriptStructDef = scriptStruct2def.FindChecked(scriptStruct);
			PostEmitStruct(pak, *scriptStructDef);
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
	EnumNames.Reserve(def.Fields.Num());
	for (const auto& enumFieldDef : def.Fields)
	{
		// Migrate from UEnum::GenerateFullEnumName(), ECppForm::EnumClass case.
		EnumNames.Emplace(FString::Printf(TEXT("%s::%s"), *enm->GetName(), *enumFieldDef.Name.ToString()), enumFieldDef.Value);
	}

	enm->SetEnums(EnumNames, UEnum::ECppForm::EnumClass, def.EnumFlags, true);
	enm->CppType = enm->GetName();
	enm->SetEnumDisplayNameFn(nullptr);

	ZUnrealFieldEmitter_Private::AddMetadata(enm, def.MetadataMap);

	// Special metadata process for UEnum.
#if WITH_METADATA
	for (const auto& enumFieldDef : def.Fields)
	{
		const TMap<FName, FString>& metadata = enumFieldDef.MetadataMap;
		if (!metadata.IsEmpty())
		{
			UMetaData* target = enm->GetOutermost()->GetMetaData();
			for (const auto& pair : metadata)
			{
				target->SetValue(enm, FName { FString::Printf(TEXT("%s.%s"), *enumFieldDef.Name.ToString(), *pair.Key.ToString()) }, *pair.Value);
			}
		}
	}
#endif

	FZSharpFieldRegistry::Get().RegisterEnum(enm);
	
	// Migrate from GetStaticEnum().
	NotifyRegistrationEvent(*enm->GetOutermost()->GetName(), *enm->GetName(), ENotifyRegistrationType::NRT_Enum, ENotifyRegistrationPhase::NRP_Finished, nullptr, false, enm);
}

void ZSharp::FZUnrealFieldEmitter::EmitStructSkeleton(UPackage* pak, FZScriptStructDefinition& def) const
{
	// UScriptStruct only has RF_Public | RF_Transient | RF_MarkAsNative set in UHT generated code,
	// but I think it's okay to keep sync with UClass.
	constexpr EObjectFlags GCompiledInFlags = RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet;

	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);
	
	FStaticConstructObjectParameters params { UScriptStruct::StaticClass() };
	params.Outer = pak;
	params.Name = *def.Name.ToString();
	params.SetFlags = def.Flags | GCompiledInFlags;
	
	auto scriptStruct = static_cast<UScriptStruct*>(StaticConstructObject_Internal(params));
	def.ScriptStruct = scriptStruct;

	FZSharpFieldRegistry::Get().RegisterScriptStruct(scriptStruct);
}

void ZSharp::FZUnrealFieldEmitter::EmitDelegateSkeleton(UPackage* pak, FZDelegateDefinition& def) const
{
	// UDelegateFunction only has RF_Public | RF_Transient | RF_MarkAsNative set in UHT generated code,
	// but I think it's okay to keep sync with UClass.
	constexpr EObjectFlags GCompiledInFlags = RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet;
	constexpr EFunctionFlags GCompiledInFunctionFlags = FUNC_Delegate;

	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);

	// @TODO: Support sparse delegate.
	check(def.DelegateType != EZDelegateType::Sparse);

	FStaticConstructObjectParameters params { def.DelegateType == EZDelegateType::Sparse ? USparseDelegateFunction::StaticClass() : UDelegateFunction::StaticClass() };
	params.Outer = def.OuterClassName.IsNone() ? static_cast<UObject*>(pak) : static_cast<UObject*>(FindObjectChecked<UClass>(pak, *def.OuterClassName.ToString()));
	params.Name = *def.Name.ToString();
	params.SetFlags = def.Flags | GCompiledInFlags;
	
	auto delegate = static_cast<UDelegateFunction*>(StaticConstructObject_Internal(params));
	def.Delegate = delegate;

	delegate->FunctionFlags |= GCompiledInFunctionFlags;
	if (def.DelegateType != EZDelegateType::Unicast)
	{
		delegate->FunctionFlags |= FUNC_MulticastDelegate;
	}

	if (def.DelegateType == EZDelegateType::Sparse)
	{
		auto sparseDelegate = CastChecked<USparseDelegateFunction>(delegate);
		sparseDelegate->OwningClassName = def.OuterClassName;
		sparseDelegate->DelegateName = "";
	}

	FZSharpFieldRegistry::Get().RegisterDelegate(delegate);
}

void ZSharp::FZUnrealFieldEmitter::EmitInterfaceSkeleton(UPackage* pak, FZInterfaceDefinition& def) const
{
	ZUnrealFieldEmitter_Private::FatalIfObjectExists(pak, def.Name);
}

void ZSharp::FZUnrealFieldEmitter::EmitClassSkeleton(UPackage* pak, FZClassDefinition& def) const
{
	// Migrate from GetPrivateStaticClassBody().
	constexpr EObjectFlags GCompiledInFlags = RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet;
	// Migrate from static constructor of UClass.
	constexpr EClassFlags GCompiledInClassFlags = CLASS_Native | CLASS_MatchedSerializers;

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
	// FCppClassTypeInfoStatic is trivially destructible so we don't need to release this manually.
	static_assert(std::is_trivially_destructible_v<FCppClassTypeInfoStatic>);
	auto typeInfo = new FCppClassTypeInfoStatic { cls->HasAllClassFlags(CLASS_Abstract) };
	cls->SetCppTypeInfoStatic(typeInfo);
	cls->ClassConfigName = def.ConfigName;

	{ // Finally setup proxy.
		FZSharpClass& zscls = FZSharpFieldRegistry::Get().RegisterClass(cls);

		zscls.bConstructor = def.bConstructor;
		zscls.bContextual = def.bContextual;

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

		if (zscls.bConstructor)
		{
			zscls.ConstructorZCallName = ZUnrealFieldEmitter_Private::MakeNMZCallName(cls, ".ctor");
		}
	}
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitStruct(UPackage* pak, FZScriptStructDefinition& def) const
{
	UScriptStruct* scriptStruct = def.ScriptStruct;

	// We didn't use static constructor before so we need to manually set up super.
	UScriptStruct* superScriptStruct = nullptr;
	if (!def.SuperPath.IsNone())
	{
		superScriptStruct = FindObjectChecked<UScriptStruct>(nullptr, *def.SuperPath.ToString());
		check(superScriptStruct->IsNative());
		scriptStruct->SetSuperStruct(superScriptStruct);
		// Migrate from UHT. (@see: UhtScriptStruct.cs)
		scriptStruct->StructFlags = static_cast<EStructFlags>(scriptStruct->StructFlags | superScriptStruct->StructFlags & STRUCT_Inherit);
		check(!FZSharpFieldRegistry::Get().IsZSharpScriptStruct(superScriptStruct) || (scriptStruct->StructFlags & STRUCT_HasInstancedReference) == STRUCT_NoFlags); // If super is a Z# script struct then it should not have STRUCT_HasInstancedReference fixed up yet.
	}

	// There is no RegisterDependencies() call for script struct.

	// Migrate from UECodeGen_Private::ConstructUScriptStruct().
	ZUnrealFieldEmitter_Private::EmitProperties(scriptStruct, def.Properties);
	
	ZUnrealFieldEmitter_Private::AddMetadata(scriptStruct, def.MetadataMap);

	// We have no way but calculate size and align like this... (Link() is idempotent, hopefully...)
	class : public FArchive
	{
		virtual FArchive& operator<<(FObjectPtr& Value) override { return *this; }
	} ar;
	scriptStruct->SetPropertiesSize(superScriptStruct ? superScriptStruct->GetPropertiesSize() : 0);
	scriptStruct->MinAlignment = superScriptStruct ? superScriptStruct->GetMinAlignment() : 1;
	// ChildProperties has been set up during EmitProperties(). (@see: UStruct::AddCppProperty())
	for (FField* field = scriptStruct->ChildProperties; field; field = field->Next)
	{
		if (field->GetOwner<UObject>() != scriptStruct)
		{
			break;
		}

		if (FProperty* property = CastField<FProperty>(field))
		{
			scriptStruct->SetPropertiesSize(property->Link(ar));
			scriptStruct->MinAlignment = FMath::Max(scriptStruct->GetMinAlignment(), property->GetMinAlignment());
		}
	}
	// Make sure the last property is aligned.
	FZSharpScriptStruct* zsstrct = FZSharpFieldRegistry::Get().GetMutableScriptStruct(scriptStruct);
	scriptStruct->SetPropertiesSize(scriptStruct->GetStructureSize());
	auto ops = new ZSharpScriptStruct_Private::FZSharpStructOps { scriptStruct, zsstrct, scriptStruct->GetPropertiesSize(), scriptStruct->GetMinAlignment() };
	scriptStruct->DeferCppStructOps(FTopLevelAssetPath { pak->GetFName(), scriptStruct->GetFName() }, ops);

	// Compile script struct.
	// All referenced script structs should be initialized so struct properties can link properly.
	scriptStruct->StaticLink(true);
	
	// Compile Z# struct.
	FZSharpScriptStruct* zsstruct = FZSharpFieldRegistry::Get().GetMutableScriptStruct(scriptStruct);
	zsstruct->CppStructOps = ops;
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitDelegate(UPackage* pak, FZDelegateDefinition& def) const
{
	UDelegateFunction* delegate = def.Delegate;

	// Migrate from UECodeGen_Private::ConstructUFunctionInternal().
	ZUnrealFieldEmitter_Private::EmitProperties(delegate, def.Properties);
	
	ZUnrealFieldEmitter_Private::AddMetadata(delegate, def.MetadataMap);

	delegate->Bind();
	delegate->StaticLink(true);

	// For nested delegate, we need to add it to outer class child link.
	if (auto outer = Cast<UClass>(delegate->GetOuter()))
	{
		// Migrate from UClass::CreateLinkAndAddChildFunctionsToMap().
		delegate->Next = outer->Children;
		outer->Children = delegate;
		outer->AddFunctionToFunctionMap(delegate, delegate->GetFName());
	}
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitInterface(UPackage* pak, FZInterfaceDefinition& def) const
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
	// @FIXME: Inherit cast flags.
	check(!FZSharpFieldRegistry::Get().IsZSharpClass(superClass) || !cls->HasAnyClassFlags(CLASS_HasInstancedReference)); // If super is a Z# class then it should not have CLASS_HasInstancedReference fixed up yet.

	auto withinClass = !def.WithinPath.IsNone() ? FindObjectChecked<UClass>(nullptr, *def.WithinPath.ToString()) : UObject::StaticClass();
	check(withinClass->IsNative());
	check(withinClass->IsChildOf(UObject::StaticClass()));
	cls->ClassWithin = withinClass;

	cls->RegisterDependencies();

	// UECodeGen_Private::ConstructUClass() calls UObjectForceRegistration() to initialize statically constructed class.
	// This process sets value for ClassPrivate, OuterPrivate, etc. and register the class to object hash.
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
		const FString mergedName = ZUnrealFieldEmitter_Private::MergeNames(lhsName, rhsName, subtractName1, subtractName2); \
		if (!mergedName.IsEmpty()) \
		{ \
			cls->SetMetaData(#LhsKey, *mergedName); \
		} \
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

void ZSharp::FZUnrealFieldEmitter::PostEmitStruct(UPackage* pak, FZScriptStructDefinition& def) const
{
	UScriptStruct* scriptStruct = def.ScriptStruct;

	/* Fixup STRUCT_HasInstancedReference for ourselves and CPF_InstancedReference | CPF_ContainsInstancedReference for our own properties.
	 * This must be deferred here because we need globally complete reflection data.
	 * For those inherit from Z# script struct in the same module, super STRUCT_HasInstancedReference should just be calculated so we propagate again here.
	 */
	if (auto superScriptStruct = CastChecked<const UScriptStruct>(scriptStruct->GetSuperStruct(), ECastCheckedType::NullAllowed))
	{
		scriptStruct->StructFlags = static_cast<EStructFlags>(scriptStruct->StructFlags | superScriptStruct->StructFlags & STRUCT_HasInstancedReference);
	}
	
	if (ZUnrealFieldEmitter_Private::FixupInstancedFlagsForStruct(scriptStruct))
	{
		scriptStruct->StructFlags = static_cast<EStructFlags>(scriptStruct->StructFlags | STRUCT_HasInstancedReference);
	}

	FZSharpScriptStruct* zsstrct = FZSharpFieldRegistry::Get().GetMutableScriptStruct(scriptStruct);
	ZUnrealFieldEmitter_Private::ConstructPropertyDefaults(def, scriptStruct, zsstrct);
}

void ZSharp::FZUnrealFieldEmitter::PostEmitDelegate(UPackage* pak, FZDelegateDefinition& def) const
{
	UDelegateFunction* delegate = def.Delegate;

	// Fixup CPF_InstancedReference | CPF_ContainsInstancedReference for our signature parameters.
	ZUnrealFieldEmitter_Private::FixupInstancedFlagsForStruct(delegate);
}

void ZSharp::FZUnrealFieldEmitter::PostEmitClass_I(UPackage* pak, FZClassDefinition& def) const
{
	UClass* cls = def.Class;

	// Fixup CPF_InstancedReference | CPF_ContainsInstancedReference for our function parameters.
	for (TFieldIterator<UFunction> it(cls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
	{
		// PostEmitDelegate() should already have fixed up delegate signature.
		if (it->HasAllFunctionFlags(FUNC_Delegate))
		{
			continue;
		}
		
		ZUnrealFieldEmitter_Private::FixupInstancedFlagsForStruct(*it);
	}

	/* Fixup CLASS_HasInstancedReference for ourselves and CPF_InstancedReference | CPF_ContainsInstancedReference for our own properties.
	 * This must be deferred here because we need globally complete reflection data.
	 * For those inherit from Z# class in the same module, super CLASS_HasInstancedReference should just be calculated so we propagate again here.
	 */
	if (auto superClass = cls->GetSuperClass())
	{
		cls->ClassFlags |= superClass->ClassFlags & CLASS_HasInstancedReference;
	}
	
	if (ZUnrealFieldEmitter_Private::FixupInstancedFlagsForStruct(cls))
	{
		cls->ClassFlags |= CLASS_HasInstancedReference;
	}

	// Compile unreal class.
	cls->Bind();
	cls->StaticLink(true);
	// Make sure the last property is aligned then mark as intrinsic.
	cls->SetPropertiesSize(cls->GetStructureSize());
	cls->SetSparseClassDataStruct(cls->GetSparseClassDataArchetypeStruct());
	cls->ClassFlags |= CLASS_Intrinsic;
	
	cls->AssembleReferenceTokenStream(true);

	// Compile Z# class.
	FZSharpClass* zscls = FZSharpFieldRegistry::Get().GetMutableClass(cls);
	// Cache DSO properties
	for (auto& defaultSubobject : zscls->DefaultSubobjects)
	{
		defaultSubobject.Property = CastFieldChecked<FObjectPropertyBase>(cls->FindPropertyByName(defaultSubobject.PropertyName));
	}
	
	ZUnrealFieldEmitter_Private::ConstructPropertyDefaults(def, cls, zscls);

	// Notify registration. Migrate from UObjectLoadAllCompiledInDefaultProperties().
	NotifyRegistrationEvent(*cls->GetOutermost()->GetName(), *cls->GetName(), ENotifyRegistrationType::NRT_Class, ENotifyRegistrationPhase::NRP_Finished, nullptr, false, cls);
}

void ZSharp::FZUnrealFieldEmitter::PostEmitClass_II(UPackage* pak, FZClassDefinition& def) const
{
	UClass* cls = def.Class;
	FZSharpClass* zscls = FZSharpFieldRegistry::Get().GetMutableClass(cls);
	const UClass* superCls = cls->GetSuperClass();
	
	// Create CDO.
	// No need to call NotifyRegistrationEvent(), UClass::CreateDefaultObject() does.
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


