// Copyright Zero Games. All Rights Reserved.


#include "ZSharpExportSettings.h"

UZSharpExportSettings::UZSharpExportSettings()
	: bExportDeprecatedFields(false)
	, bTreatDeprecatedPostfixAsDeprecated(true)
	, bExportEditorOnlyFields(false)
{
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.PlayerController:GetDeprecatedInputYawScale");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.PlayerController:GetDeprecatedInputPitchScale");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.PlayerController:GetDeprecatedInputRollScale");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.PlayerController:SetDeprecatedInputYawScale");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.PlayerController:SetDeprecatedInputPitchScale");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.PlayerController:SetDeprecatedInputRollScale");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.WorldDataLayers:DeprecatedDataLayerNameToDataLayerInstance");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.DeprecatedDataLayerInstance");
	IntrinsicForceNotExportFieldPaths.Add("/Script/Engine.EdGraphNode:DeprecatedPins");
}

void UZSharpExportSettings::PostInitProperties()
{
	Super::PostInitProperties();

	InvalidateCache();
}

void UZSharpExportSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);
	
	InvalidateCache();
}

#if WITH_EDITOR

void UZSharpExportSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	InvalidateCache();
}

void UZSharpExportSettings::InvalidateCache()
{
	ForceExportFieldPathsHash.Reset();
	for (const auto& path : ForceExportFieldPaths)
	{
		ForceExportFieldPathsHash.Add(path);
	}
	
	ForceNotExportFieldPathsHash.Reset();
    for (const auto& path : ForceNotExportFieldPaths)
    {
    	ForceNotExportFieldPathsHash.Add(path);
    }
}

#endif


