// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedTypeDto.h"
#include "ZExportedParameterDto.h"
#include "ZFullyExportedTypeNameDto.h"

#include "ZExportedClassDto.generated.h"

USTRUCT()
struct FZExportedMethodDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString ZCallName;

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	TArray<FZExportedParameterDto> Parameters;
};

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
	FString UnderlyingType;
	
	UPROPERTY()
	bool bNullInNotNullOut = false;

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
	TArray<FZFullyExportedTypeNameDto> Interfaces;

	UPROPERTY()
	TArray<FZExportedMethodDto> Methods;

	UPROPERTY()
	TArray<FZExportedPropertyDto> Properties;
};


