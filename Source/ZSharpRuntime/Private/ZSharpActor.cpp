// Copyright Zero Games. All Rights Reserved.


#include "ZSharpActor.h"

#include "ZConjugateRegistry.h"
#include "CLR/IZSharpCLR.h"
#include "ALC/IZAssembly.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/IZMethodInfo.h"
#include "ALC/IZType.h"
#include "ZCall/ZCallBuffer.h"

AZSharpActor::AZSharpActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZSharpActor::BeginPlay()
{
	Super::BeginPlay();

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterAlc();
	if (alc)
	{
		const ZSharp::IZAssembly* assembly = alc->GetAssembly("ZeroGames.ZSharp.UnrealEngine");
		const ZSharp::IZType* type = assembly->GetType("ZeroGames.ZSharp.UnrealEngine.ZSharpActorStatics");
		const ZSharp::IZMethodInfo* beginplay = type->GetMethod("BeginPlay");
		
		beginplay->Invoke([this](ZSharp::FZCallBuffer& buffer, int32 numSlots)
		{
			buffer.Slots[0].Conjugate = ZSharp::FZConjugateRegistry::Get()->Conjugate(this);
		});
	}
}

void AZSharpActor::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterAlc();
	if (alc)
	{
		const ZSharp::IZAssembly* assembly = alc->GetAssembly("ZeroGames.ZSharp.UnrealEngine");
		const ZSharp::IZType* type = assembly->GetType("ZeroGames.ZSharp.UnrealEngine.ZSharpActorStatics");
		const ZSharp::IZMethodInfo* endplay = type->GetMethod("EndPlay");

		endplay->Invoke([this](ZSharp::FZCallBuffer& buffer, int32 numSlots)
		{
			buffer.Slots[0].Conjugate = ZSharp::FZConjugateRegistry::Get()->Conjugate(this);
		});
	}
	
	Super::EndPlay(endPlayReason);
}

void AZSharpActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterAlc();
	if (alc)
	{
		const ZSharp::IZAssembly* assembly = alc->GetAssembly("ZeroGames.ZSharp.UnrealEngine");
		const ZSharp::IZType* type = assembly->GetType("ZeroGames.ZSharp.UnrealEngine.ZSharpActorStatics");
		const ZSharp::IZMethodInfo* tick = type->GetMethod("Tick");
		
		tick->Invoke([this, deltaTime](ZSharp::FZCallBuffer& buffer, int32 numSlots)
		{
			buffer.Slots[0].Conjugate = ZSharp::FZConjugateRegistry::Get()->Conjugate(this);
			buffer.Slots[1].Float = deltaTime;
		});
	}
}

