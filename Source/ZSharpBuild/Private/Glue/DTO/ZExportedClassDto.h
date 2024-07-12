// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedTypeDto.h"
#include "ZFullyExportedTypeNameDto.h"

#include "ZExportedClassDto.generated.h"

USTRUCT()
struct FZExportedPropertyDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString ZCallName;

	UPROPERTY()
	FZFullyExportedTypeNameDto Type;

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	int32 Index = 0;
};

USTRUCT()
struct FZExportedClassDto : public FZExportedTypeDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 ConjugateRegistryId = 0;

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	FZFullyExportedTypeNameDto BaseType;

	UPROPERTY()
	TMap<FString, FZExportedPropertyDto> PropertyMap;
};


