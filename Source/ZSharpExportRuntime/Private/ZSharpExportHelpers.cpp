// Copyright Zero Games. All Rights Reserved.


#include "ZSharpExportHelpers.h"

#include "ZSharpExportRuntimeSettings.h"

FString ZSharp::FZSharpExportHelpers::GetUFieldAliasedName(const UField* field)
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
		if (cls->HasAnyClassFlags(CLASS_Interface))
		{
			name.InsertAt(0, 'I');
		}
	}

	return name;
}

FString ZSharp::FZSharpExportHelpers::GetUFieldModuleName(const UField* field)
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

bool ZSharp::FZSharpExportHelpers::IsUFieldModuleMapped(const UField* field)
{
	return GetDefault<UZSharpExportRuntimeSettings>()->IsModuleMapped(GetUFieldModuleName(field));
}

const UField* ZSharp::FZSharpExportHelpers::GetUFieldClosestMappedAncestor(const UField* field)
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

FString ZSharp::FZSharpExportHelpers::GetUFieldOuterExportName(const UField* field)
{
	return GetUFieldInnerExportName(GetUFieldClosestMappedAncestor(field));
}

FString ZSharp::FZSharpExportHelpers::GetUFieldInnerExportName(const UField* field)
{
	if (!field)
	{
		return {};
	}
	
	const FString name = GetUFieldAliasedName(field);
	const FString module = GetUFieldModuleName(field);
	return FString::Printf(TEXT("%s.%s"), *module, *name);
}

bool ZSharp::FZSharpExportHelpers::GetUFieldRuntimeTypeLocatorInfo(const UField* field, FString& assemblyName, FString& typeName)
{
	const UField* ancestor = GetUFieldClosestMappedAncestor(field);
	if (!GetDefault<UZSharpExportRuntimeSettings>()->TryGetModuleAssembly(GetUFieldModuleName(ancestor), assemblyName))
	{
		return false;
	}
	
	typeName = GetUFieldInnerExportName(ancestor);
	if (!typeName.Len())
	{
		return false;
	}

	return true;
}

bool ZSharp::FZSharpExportHelpers::GetFPropertyRuntimeTypeLocatorInfo(const FProperty* property, FString& assemblyName, FString& typeName)
{
	return false;
}


