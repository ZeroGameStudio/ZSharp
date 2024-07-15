// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ZSharpUserWidget.generated.h"

USTRUCT()
struct FZSharpUserWidgetZCallName
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString AssemblyName;

	UPROPERTY(EditAnywhere)
	FString ClassName;

	UPROPERTY(EditAnywhere)
	FString MethodName;

	FString operator*() const
	{
		return FString::Printf(TEXT("m://%s:%s:%s"), *AssemblyName, *ClassName, *MethodName);
	}
};

UCLASS(Abstract)
class ZSHARPRUNTIME_API UZSharpUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UZSharpUserWidget(const FObjectInitializer& objectInitializer);

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& geometry, float deltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	FZSharpUserWidgetZCallName OnInitializedZCall;
	
	UPROPERTY(EditDefaultsOnly)
	FZSharpUserWidgetZCallName ConstructZCall;

	UPROPERTY(EditDefaultsOnly)
	FZSharpUserWidgetZCallName DestructZCall;

	UPROPERTY(EditDefaultsOnly)
	FZSharpUserWidgetZCallName TickZCall;
	
};


