// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeNameDto.h"

#include "ZExportedParameterDto.generated.h"

USTRUCT()
struct FZExportedParameterDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FZFullyExportedTypeNameDto Type;

	UPROPERTY()
	uint64 Flags = 0;
};