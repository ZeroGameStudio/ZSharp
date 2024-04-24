// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSharpActor.generated.h"

UCLASS()
class ZSHARPRUNTIME_API AZSharpActor : public AActor
{
	GENERATED_BODY()

public:
	AZSharpActor();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:
	virtual void Tick(float deltaTime) override;
	
};
