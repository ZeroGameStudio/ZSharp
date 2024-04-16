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

void AZSharpActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AZSharpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

