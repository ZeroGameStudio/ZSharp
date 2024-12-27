// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedParameterDto.h"

#include "ZExportedMethodDto.generated.h"

USTRUCT()
struct FZExportedMethodDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString ZCallName;

	UPROPERTY()
	FZFullyExportedTypeNameDto OwnerInterface;

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	TArray<FZExportedParameterDto> Parameters;
};


