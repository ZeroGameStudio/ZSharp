// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicTypeExporter.h"

#include "ZSharpRuntimeSettings.h"
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

		FString module;
		bool bSuc = enm->GetPackage()->GetName().Split("/", nullptr, &module, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (!bSuc || !GetDefault<UZSharpRuntimeSettings>()->IsModuleMapped(module))
		{
			continue;
		}

		const auto* exportedEnum = new FZDynamicExportedEnum { enm };
		if (!exportedEnum->IsRegistered())
		{
			delete exportedEnum;
		}
	}
}


