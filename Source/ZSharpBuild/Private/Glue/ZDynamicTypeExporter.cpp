// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicTypeExporter.h"

#include "ZSharpExportHelpers.h"
#include "ZSharpExportRuntimeSettings.h"
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
		if (!GetDefault<UZSharpExportRuntimeSettings>()->IsModuleMapped(module))
		{
			continue;
		}

		FZDynamicExportedEnum::Create(enm);
	}

	for (TObjectIterator<UScriptStruct> It; It; ++It)
	{
		UScriptStruct* strct = *It;
		if (!strct->IsNative())
		{
			continue;
		}

		FString module = FZSharpExportHelpers::GetUFieldModuleName(strct);
		if (!GetDefault<UZSharpExportRuntimeSettings>()->IsModuleMapped(module))
		{
			continue;
		}

		FZDynamicExportedClass::Create(strct);
	}

	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* cls = *It;
		if (!cls->IsNative())
		{
			continue;
		}

		FString module = FZSharpExportHelpers::GetUFieldModuleName(cls);
		if (!GetDefault<UZSharpExportRuntimeSettings>()->IsModuleMapped(module))
		{
			continue;
		}

		FZDynamicExportedClass::Create(cls);
	}
}


