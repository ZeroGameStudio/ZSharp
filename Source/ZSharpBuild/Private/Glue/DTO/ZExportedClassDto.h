// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedTypeDto.h"
#include "ZExportedMethodDto.h"
#include "ZExportedPropertyDto.h"
#include "ZFullyExportedTypeNameDto.h"

#include "ZExportedClassDto.generated.h"

USTRUCT()
struct FZExportedClassDto : public FZExportedTypeDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 ConjugateRegistryId = 0;

	UPROPERTY()
	FString ConjugateKey;

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	FZFullyExportedTypeNameDto BaseType;

	UPROPERTY()
	TArray<FZFullyExportedTypeNameDto> Interfaces;

	UPROPERTY()
	TArray<FZExportedMethodDto> Methods;

	UPROPERTY()
	TArray<FZExportedPropertyDto> Properties;
};


