// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedVariableDto.h"

#include "ZExportedPropertyDto.generated.h"

USTRUCT()
struct FZExportedPropertyDto : public FZExportedVariableDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString ZCallName;

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	int32 Index = 0;
};


