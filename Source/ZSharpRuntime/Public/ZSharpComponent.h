// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpComponent.generated.h"

USTRUCT()
struct FZSharpComponentZCallName
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

UCLASS(meta = (BlueprintSpawnableComponent))
class ZSHARPRUNTIME_API UZSharpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UZSharpComponent();
	
private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly)
	FZSharpComponentZCallName BeginPlayZCall;

	UPROPERTY(EditDefaultsOnly)
	FZSharpComponentZCallName EndPlayZCall;

	UPROPERTY(EditDefaultsOnly)
	FZSharpComponentZCallName TickZCall;
	
};
