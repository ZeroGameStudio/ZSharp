// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedTypeDto.h"

#include "ZExportedClassDto.generated.h"

USTRUCT()
struct FZExportedClassDto : public FZExportedTypeDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 Flags;

	UPROPERTY()
	FString BaseType;
};


