﻿// Copyright Zero Games. All Rights Reserved.


#include "Reflection/ZReflectionHelper.h"

#include "ZSharpRuntimeSettings.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Trait/ZManagedTypeInfo.h"
#include "UObject/PropertyOptional.h"

FString ZSharp::FZReflectionHelper::GetUFieldAliasedName(const UField* field)
{
	static const TMap<FString, FString> GAliasMap
	{
			{ "Object", "UnrealObject" },
		
			{ "Field", "UnrealField" },
			{ "Struct", "UnrealStruct" },
			{ "Class", "UnrealClass" },
			{ "Interface", "UnrealInterface" },
			{ "ScriptStruct", "UnrealScriptStruct" },
			{ "Enum", "UnrealEnum" },
		
			{ "Function", "UnrealFunction" },
			{ "Property", "UnrealProperty" },

			{ "Guid", "UnrealGuid" },
	};

	if (!field)
	{
		return {};
	}

	FString name = field->GetName();
	if (const FString* alias = GAliasMap.Find(name))
	{
		name = *alias;
	}
	
	if (auto cls = Cast<UClass>(field))
	{
		if (cls->HasAllClassFlags(CLASS_Interface))
		{
			name.InsertAt(0, 'I');
		}
	}
	else if (auto func = Cast<UFunction>(field))
	{
		static const FString GDelegatePostfix = "__DelegateSignature";
		if (func->HasAllFunctionFlags(FUNC_Delegate) && name.EndsWith(GDelegatePostfix))
		{
			name.LeftChopInline(GDelegatePostfix.Len());
		}
	}

	if (const UField* outer = field->GetTypedOuter<UField>())
	{
		name = GetUFieldAliasedName(outer).Append(".").Append(name);
	}

	return name;
}

FString ZSharp::FZReflectionHelper::GetUFieldAssemblyName(const UField* field)
{
	const FString moduleName = GetUFieldModuleName(field);
	FString res;
	if (!GetDefault<UZSharpRuntimeSettings>()->TryGetModuleAssembly(moduleName, res))
	{
		return {};
	}

	return res;
}

FString ZSharp::FZReflectionHelper::GetUFieldModuleName(const UField* field)
{
	if (!field)
	{
		return {};
	}
	
	FString res;
	const bool suc = field->GetPackage()->GetName().Split("/", nullptr, &res, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	check(suc);
	return res;
}

bool ZSharp::FZReflectionHelper::IsUFieldModuleMapped(const UField* field)
{
	return GetDefault<UZSharpRuntimeSettings>()->IsModuleMapped(GetUFieldModuleName(field));
}

const UField* ZSharp::FZReflectionHelper::GetUFieldClosestMappedAncestor(const UField* field)
{
	if (!field)
	{
		return nullptr;
	}
	
	if (IsUFieldModuleMapped(field))
	{
		return field;
	}
	
	auto strct = Cast<UStruct>(field);
	if (strct)
	{
		for (const UStruct* current = strct->GetSuperStruct(); current; current = current->GetSuperStruct())
		{
			if (IsUFieldModuleMapped(current))
			{
				return current;
			}
		}
	}

	return nullptr;
}

bool ZSharp::FZReflectionHelper::GetUFieldRuntimeTypeLocator(const UField* field, FZRuntimeTypeLocatorWrapper& outLocator)
{
	const UField* ancestor = GetUFieldClosestMappedAncestor(field);
	const FString moduleName = GetUFieldModuleName(ancestor);
	if (!moduleName.Len())
	{
		return false;
	}
	
	outLocator.AssemblyName = GetUFieldAssemblyName(ancestor);
	if (!outLocator.AssemblyName.Len())
	{
		return false;
	}
	
	outLocator.TypeName = FString::Printf(TEXT("%s.%s.%s"), *outLocator.AssemblyName, *moduleName, *GetUFieldAliasedName(ancestor));

	return true;
}

bool ZSharp::FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(const FFieldClass* cls, FZRuntimeTypeLocatorWrapper& outLocator)
{
	static const TMap<FFieldClass*, FZRuntimeTypeLocatorWrapper> GProtoMap
	{
		{ FByteProperty::StaticClass(), { TZManagedTypeInfo<uint8>::GetAssemblyName(), TZManagedTypeInfo<uint8>::GetFullName() } },
		{ FUInt16Property::StaticClass(), { TZManagedTypeInfo<uint16>::GetAssemblyName(), TZManagedTypeInfo<uint16>::GetFullName() } },
		{ FUInt32Property::StaticClass(), { TZManagedTypeInfo<uint32>::GetAssemblyName(), TZManagedTypeInfo<uint32>::GetFullName() } },
		{ FUInt64Property::StaticClass(), { TZManagedTypeInfo<uint64>::GetAssemblyName(), TZManagedTypeInfo<uint64>::GetFullName() } },
		{ FInt8Property::StaticClass(), { TZManagedTypeInfo<int8>::GetAssemblyName(), TZManagedTypeInfo<int8>::GetFullName() } },
		{ FInt16Property::StaticClass(), { TZManagedTypeInfo<int16>::GetAssemblyName(), TZManagedTypeInfo<int16>::GetFullName() } },
		{ FIntProperty::StaticClass(), { TZManagedTypeInfo<int32>::GetAssemblyName(), TZManagedTypeInfo<int32>::GetFullName() } },
		{ FInt64Property::StaticClass(), { TZManagedTypeInfo<int64>::GetAssemblyName(), TZManagedTypeInfo<int64>::GetFullName() } },
		{ FFloatProperty::StaticClass(), { TZManagedTypeInfo<float>::GetAssemblyName(), TZManagedTypeInfo<float>::GetFullName() } },
		{ FDoubleProperty::StaticClass(), { TZManagedTypeInfo<double>::GetAssemblyName(), TZManagedTypeInfo<double>::GetFullName() } },
		{ FBoolProperty::StaticClass(), { TZManagedTypeInfo<bool>::GetAssemblyName(), TZManagedTypeInfo<bool>::GetFullName() } },

		{ FStrProperty::StaticClass(), { TZManagedTypeInfo<FString>::GetAssemblyName(), TZManagedTypeInfo<FString>::GetFullName() } },
		{ FNameProperty::StaticClass(), { TZManagedTypeInfo<FName>::GetAssemblyName(), TZManagedTypeInfo<FName>::GetFullName() } },
		{ FTextProperty::StaticClass(), { TZManagedTypeInfo<FText>::GetAssemblyName(), TZManagedTypeInfo<FText>::GetFullName() } },

		{ FArrayProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptArray>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptArray>::GetFullName() } },
		{ FSetProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptSet>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptSet>::GetFullName() } },
		{ FMapProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptMap>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptMap>::GetFullName() } },
		{ FOptionalProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveOptional>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveOptional>::GetFullName() } },

		{ FDelegateProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptDelegate>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptDelegate>::GetFullName() } },
		{ FMulticastInlineDelegateProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveMulticastScriptDelegate>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveMulticastScriptDelegate>::GetFullName() } },
		{ FMulticastSparseDelegateProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveMulticastScriptDelegate>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveMulticastScriptDelegate>::GetFullName() } },

		{ FClassProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveSubclassOf>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveSubclassOf>::GetFullName() } },
		{ FSoftClassProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveSoftClassPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveSoftClassPtr>::GetFullName() } },
		{ FSoftObjectProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveSoftObjectPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveSoftObjectPtr>::GetFullName() } },
		{ FWeakObjectProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveWeakObjectPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveWeakObjectPtr>::GetFullName() } },
		{ FLazyObjectProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveLazyObjectPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveLazyObjectPtr>::GetFullName() } },
		{ FInterfaceProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptInterface>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptInterface>::GetFullName() } },

		{ FFieldPathProperty::StaticClass(), { TZManagedTypeInfo<FFieldPath>::GetAssemblyName(), TZManagedTypeInfo<FFieldPath>::GetFullName() } },
	};

	const FZRuntimeTypeLocatorWrapper* proto = GProtoMap.Find(cls);
	if (!proto)
	{
		checkNoEntry()
		return false;
	}

	outLocator = *proto;
	return true;
}

bool ZSharp::FZReflectionHelper::GetFPropertyRuntimeTypeLocator(const FProperty* property, FZRuntimeTypeLocatorWrapper& outLocator)
{
	if (const auto classProp = CastField<FClassProperty>(property))
	{
		if (classProp->MetaClass == UObject::StaticClass())
		{
			return GetUFieldRuntimeTypeLocator(UClass::StaticClass(), outLocator);
		}
	}
	else if (const auto objectProp = CastField<FObjectProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(objectProp->PropertyClass, outLocator);
	}
	else if (const auto structProp = CastField<FStructProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(structProp->Struct, outLocator);
	}
	else if (const auto enumProp = CastField<FEnumProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(enumProp->GetEnum(), outLocator);
	}
	
	if (!GetFFieldClassRuntimeTypeLocator(property->GetClass(), outLocator))
	{
		return false;
	}

	if (outLocator.TypeName.Contains("`"))
	{
		FZRuntimeTypeLocatorWrapper& inner = outLocator.TypeParameters.Emplace_GetRef();
		if (const auto classProp = CastField<FClassProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(classProp->MetaClass, inner);
		}
		else if (const auto objectProp = CastField<FObjectProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(objectProp->PropertyClass, inner);
		}
	}
	
	checkNoEntry();
	return false;
}
