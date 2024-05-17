// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicTypeExporter.h"

#include "ZSharpExportHelpers.h"
#include "ZSharpExportSettings.h"
#include "Dynamic/ZDynamicExportedClass.h"
#include "Dynamic/ZDynamicExportedEnum.h"

void ZSharp::FZDynamicTypeExporter::Export() const
{
	for (TObjectIterator<UEnum> It; It; ++It)
	{
		UEnum* enm = *It;
		if (!enm->IsNative())
		{
			continue;
		}

		FString module = FZSharpExportHelpers::GetUFieldModuleName(enm);
		if (!GetDefault<UZSharpExportSettings>()->IsModuleMapped(module))
		{
			continue;
		}

		const auto* exportedEnum = new FZDynamicExportedEnum { enm };
		if (!exportedEnum->IsRegistered())
		{
			delete exportedEnum;
		}
	}

	for (TObjectIterator<UScriptStruct> It; It; ++It)
	{
		UScriptStruct* strct = *It;
		if (!strct->IsNative())
		{
			continue;
		}

		FString module = FZSharpExportHelpers::GetUFieldModuleName(strct);
		if (!GetDefault<UZSharpExportSettings>()->IsModuleMapped(module))
		{
			continue;
		}

		const auto* exportedClass = new FZDynamicExportedClass { strct };
		if (!exportedClass->IsRegistered())
		{
			delete exportedClass;
		}
	}

	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* cls = *It;
		if (!cls->IsNative())
		{
			continue;
		}

		FString module = FZSharpExportHelpers::GetUFieldModuleName(cls);
		if (!GetDefault<UZSharpExportSettings>()->IsModuleMapped(module))
		{
			continue;
		}

		const auto* exportedClass = new FZDynamicExportedClass { cls };
		if (!exportedClass->IsRegistered())
		{
			delete exportedClass;
		}
	}
}


