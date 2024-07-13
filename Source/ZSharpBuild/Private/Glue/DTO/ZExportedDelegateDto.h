// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedParameterDto.h"
#include "ZExportedTypeDto.h"

#include "ZExportedDelegateDto.generated.h"

USTRUCT()
struct FZExportedDelegateDto : public FZExportedTypeDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 Flags = 0;

	UPROPERTY()
	TArray<FZExportedParameterDto> Parameters;
};


