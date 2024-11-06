// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedValue.h"

#include "ZExportedValueDto.generated.h"

USTRUCT()
struct FZExportedValueDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Text;

	FZExportedValueDto& operator=(const ZSharp::FZExportedValue& other)
	{
		Text = other.Text;
		
		return *this;
	}
	
};


