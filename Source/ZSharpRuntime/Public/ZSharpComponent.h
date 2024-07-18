// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpComponent.generated.h"

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
