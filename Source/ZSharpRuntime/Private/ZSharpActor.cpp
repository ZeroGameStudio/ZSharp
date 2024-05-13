// Copyright Zero Games. All Rights Reserved.


#include "ZSharpActor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/Conjugate/ZConjugateRegistry_UObject.h"

AZSharpActor::AZSharpActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZSharpActor::BeginPlay()
{
	Super::BeginPlay();

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)));
		
		alc->ZCall({}, &buffer);
	}
}

void AZSharpActor::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)));
		
		alc->ZCall({}, &buffer);
	}
	
	Super::EndPlay(endPlayReason);
}

void AZSharpActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(
			ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)),
			ZSharp::FZCallBufferSlot::FromFloat(deltaTime));
		
		alc->ZCall({}, &buffer);
	}
}

