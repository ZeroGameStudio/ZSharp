// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicTypeExporter.h"

#include "ZSharpExportHelpers.h"
#include "ZSharpExportRuntimeSettings.h"
#include "Dynamic/ZDynamicallyExportedClass.h"
#include "Dynamic/ZDynamicallyExportedEnum.h"

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

		FZDynamicallyExportedEnum::Create(enm);
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

		FZDynamicallyExportedClass::Create(strct);
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

		FZDynamicallyExportedClass::Create(cls);
	}
}


