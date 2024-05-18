// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedClassDto.h"
#include "ZExportedEnumDto.h"

#include "ZExportedAssemblyDto.generated.h"

USTRUCT()
struct FZExportedAssemblyDto
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FZExportedEnumDto> Enums;

	UPROPERTY()
	TArray<FZExportedClassDto> Classes;
};


