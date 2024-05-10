// Copyright Zero Games. All Rights Reserved.


#include "ZSharpActor.h"

AZSharpActor::AZSharpActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZSharpActor::BeginPlay()
{
	Super::BeginPlay();
}

void AZSharpActor::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);
}

void AZSharpActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

