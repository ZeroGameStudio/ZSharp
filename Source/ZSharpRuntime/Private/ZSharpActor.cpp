// Copyright Zero Games. All Rights Reserved.


#include "ZSharpActor.h"

#include "ZConjugateRegistry.h"
#include "CLR/IZSharpCLR.h"
#include "Interop/IZAssembly.h"
#include "Interop/IZMasterAssemblyLoadContext.h"
#include "Interop/IZMethodInfo.h"
#include "Interop/IZType.h"
#include "Interop/ZCallBuffer.h"

AZSharpActor::AZSharpActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZSharpActor::BeginPlay()
{
	Super::BeginPlay();

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterALC();
	if (alc)
	{
		const ZSharp::IZAssembly* assembly = alc->GetAssembly("ZeroGames.ZSharp.UnrealEngine");
		const ZSharp::IZType* type = assembly->GetType("ZeroGames.ZSharp.UnrealEngine.ZSharpActorStatics");
		const ZSharp::IZMethodInfo* beginplay = type->GetMethod("BeginPlay");
		
		ZSharp::FZCallBufferSlot slots[1];
		slots[0].Conjugate = ZSharp::FZConjugateRegistry::Get().BuildConjugate(this);
		ZSharp::FZCallBuffer buffer { slots };
		
		beginplay->Invoke(&buffer);
	}
}

void AZSharpActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterALC();
	if (alc)
	{
		const ZSharp::IZAssembly* assembly = alc->GetAssembly("ZeroGames.ZSharp.UnrealEngine");
		const ZSharp::IZType* type = assembly->GetType("ZeroGames.ZSharp.UnrealEngine.ZSharpActorStatics");
		const ZSharp::IZMethodInfo* beginplay = type->GetMethod("EndPlay");
		
		ZSharp::FZCallBufferSlot slots[1];
		slots[0].Conjugate = ZSharp::FZConjugateRegistry::Get().BuildConjugate(this);
		ZSharp::FZCallBuffer buffer { slots };
		
		beginplay->Invoke(&buffer);
	}
	
	Super::EndPlay(EndPlayReason);
}

void AZSharpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterALC();
	if (alc)
	{
		const ZSharp::IZAssembly* assembly = alc->GetAssembly("ZeroGames.ZSharp.UnrealEngine");
		const ZSharp::IZType* type = assembly->GetType("ZeroGames.ZSharp.UnrealEngine.ZSharpActorStatics");
		const ZSharp::IZMethodInfo* beginplay = type->GetMethod("Tick");
		
		ZSharp::FZCallBufferSlot slots[2];
		slots[0].Conjugate = ZSharp::FZConjugateRegistry::Get().BuildConjugate(this);
		slots[1].Float = DeltaTime;
		ZSharp::FZCallBuffer buffer { slots };
		
		beginplay->Invoke(&buffer);
	}
}

