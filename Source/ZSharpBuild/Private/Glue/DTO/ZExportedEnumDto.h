// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedTypeDto.h"

#include "ZExportedEnumDto.generated.h"

USTRUCT()
struct FZExportedEnumDto : public FZExportedTypeDto
{
	GENERATED_BODY()

	FZExportedEnumDto()
		: Flags(0){}

	UPROPERTY()
	uint64 Flags;

	UPROPERTY()
	FString UnderlyingType;

	UPROPERTY()
	TMap<FString, FString> ValueMap;
};


