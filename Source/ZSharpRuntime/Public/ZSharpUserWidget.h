// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ZSharpUserWidget.generated.h"

UCLASS(Abstract)
class ZSHARPRUNTIME_API UZSharpUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& geometry, float deltaTime) override;

private:
	FString MakeZCallName(const FString& MethodName) const
	{
		return FString::Printf(TEXT("m://%s:%s:%s"), *AssemblyName, *ClassName, *MethodName);
	}

private:
	UPROPERTY(EditDefaultsOnly)
	FString AssemblyName;

	UPROPERTY(EditDefaultsOnly)
	FString ClassName;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShouldCallManagedTick;
	
};


