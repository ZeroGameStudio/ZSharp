// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ZSharpRuntimeSettings.generated.h"

/**
 * 
 */
UCLASS(Config = ZSharp, DefaultConfig)
class ZSHARPRUNTIME_API UZSharpRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UZSharpRuntimeSettings();

public:
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }
	
};


