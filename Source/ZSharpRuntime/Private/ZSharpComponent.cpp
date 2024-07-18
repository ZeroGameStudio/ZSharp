// Copyright Zero Games. All Rights Reserved.


#include "ZSharpComponent.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

UZSharpComponent::UZSharpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UZSharpComponent::BeginPlay()
{
	Super::BeginPlay();

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)));

		ZSharp::FZCallHandle handle = alc->GetZCallHandle(MakeZCallName("BeginPlay"));
		alc->ZCall(handle, &buffer);
	}
}

void UZSharpComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)));

		ZSharp::FZCallHandle handle = alc->GetZCallHandle(MakeZCallName("EndPlay"));
		alc->ZCall(handle, &buffer);
	}
	
	Super::EndPlay(endPlayReason);
}

void UZSharpComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (!bShouldCallManagedTick)
	{
		return;
	}

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (alc)
	{
		alc->PrepareForZCall();
		ZSHARP_STACK_ALLOC_ZCALL_BUFFER(
			ZSharp::FZCallBufferSlot::FromConjugate(alc->GetConjugateRegistry<ZSharp::FZConjugateRegistry_UObject>().Conjugate(this)),
			ZSharp::FZCallBufferSlot::FromFloat(deltaTime));

		ZSharp::FZCallHandle handle = alc->GetZCallHandle(MakeZCallName("Tick"));
		alc->ZCall(handle, &buffer);
	}
}
