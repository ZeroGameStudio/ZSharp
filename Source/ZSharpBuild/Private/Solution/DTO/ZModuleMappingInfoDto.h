// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZModuleMappingInfoDto.generated.h"

USTRUCT()
struct FZModuleMappingInfoDto
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FString, FString> Mapping;
};


