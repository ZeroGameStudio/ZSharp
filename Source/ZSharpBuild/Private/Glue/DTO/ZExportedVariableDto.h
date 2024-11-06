// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeNameDto.h"

#include "ZExportedVariableDto.generated.h"

USTRUCT()
struct FZExportedVariableDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FZFullyExportedTypeNameDto Type;

	UPROPERTY()
	FString UnderlyingType;

	UPROPERTY()
	bool bNullInNotNullOut = false;
};


