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
	FString res;
	const bool suc = field->GetPackage()->GetName().Split("/", nullptr, &res, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	check(suc);
	return res;
}

bool ZSharp::FZSharpExportHelpers::IsUFieldModuleMapped(const UField* field)
{
	return GetDefault<UZSharpExportRuntimeSettings>()->IsModuleMapped(GetUFieldModuleName(field));
}

FString ZSharp::FZSharpExportHelpers::GetUFieldOuterExportName(const UField* field)
{
	if (IsUFieldModuleMapped(field))
	{
		return GetUFieldInnerExportName(field);
	}

	const auto strct = Cast<UStruct>(field);
	if (strct)
	{
		for (const UStruct* current = strct->GetSuperStruct(); current; current = current->GetSuperStruct())
		{
			if (IsUFieldModuleMapped(current))
			{
				return GetUFieldInnerExportName(current);
			}
		}
	}

	return {};
}

FString ZSharp::FZSharpExportHelpers::GetUFieldInnerExportName(const UField* field)
{
	const FString name = GetUFieldAliasedName(field);
	const FString module = GetUFieldModuleName(field);
	return FString::Printf(TEXT("%s.%s"), *module, *name);
}


