// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedDefaultValue.h"
#include "ZExportedValueDto.h"

#include "ZExportedDefaultValueDto.generated.h"

USTRUCT()
struct FZExportedDefaultValueDto
{
	GENERATED_BODY()

	UPROPERTY()
	FZExportedValueDto Signature;

	UPROPERTY()
	FZExportedValueDto Body;

	FZExportedDefaultValueDto& operator=(const ZSharp::FZExportedDefaultValue& other)
	{
		Signature = other.Signature;
		Body = other.Body;
		
		return *this;
	}
	
};


