// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedDefaultValueDto.h"
#include "ZExportedVariableDto.h"

#include "ZExportedParameterDto.generated.h"

USTRUCT()
struct FZExportedParameterDto : public FZExportedVariableDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	FZExportedDefaultValueDto DefaultValue;
};