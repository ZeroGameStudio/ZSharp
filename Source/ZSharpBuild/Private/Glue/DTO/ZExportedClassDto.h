// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedTypeDto.h"

#include "ZExportedClassDto.generated.h"

USTRUCT()
struct FZExportedPropertyDto
{
	GENERATED_BODY()

	FZExportedPropertyDto()
		: Flags(0)
		, Index(0){}

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString ZCallName;

	UPROPERTY()
	FString Type;

	UPROPERTY()
	uint64 Flags;

	UPROPERTY()
	int32 Index;
};

USTRUCT()
struct FZExportedClassDto : public FZExportedTypeDto
{
	GENERATED_BODY()

	FZExportedClassDto()
		: ConjugateRegistryId(0)
		, Flags(0){}

	UPROPERTY()
	uint16 ConjugateRegistryId;

	UPROPERTY()
	uint64 Flags;

	UPROPERTY()
	FString BaseType;

	UPROPERTY()
	TMap<FString, FZExportedPropertyDto> PropertyMap;
};


