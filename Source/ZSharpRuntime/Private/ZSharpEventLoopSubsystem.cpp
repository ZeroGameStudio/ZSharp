// Copyright Zero Games. All Rights Reserved.


#include "ZSharpEventLoopSubsystem.h"

#include "Interop/ZSharpEventLoop_Interop.h"

void FZSharpEventLoopTickFunction::Run() const
{
	static const TMap<ETickingGroup, ZSharp::EZSharpEventLoopEventType> GTypeMap
	{
		{ TG_PrePhysics, ZSharp::EZSharpEventLoopEventType::PrePhysicsTick },
		{ TG_DuringPhysics, ZSharp::EZSharpEventLoopEventType::DuringPhysicsTick },
		{ TG_PostPhysics, ZSharp::EZSharpEventLoopEventType::PostPhysicsTick },
		{ TG_PostUpdateWork, ZSharp::EZSharpEventLoopEventType::PostUpdateTick },
	};

	Owner->NotifyEvent(GTypeMap[TickGroup]);
}

UZSharpEventLoopSubsystem::UZSharpEventLoopSubsystem()
	: PrePhysicsTickFunction(MakeUnique<FZSharpEventLoopTickFunction>(this, TG_PrePhysics))
	, DuringPhysicsTickFunction(MakeUnique<FZSharpEventLoopTickFunction>(this, TG_DuringPhysics))
	, PostPhysicsTickFunction(MakeUnique<FZSharpEventLoopTickFunction>(this, TG_PostPhysics))
	, PostUpdateTickFunction(MakeUnique<FZSharpEventLoopTickFunction>(this, TG_PostUpdateWork))
{
}

void UZSharpEventLoopSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	FWorldDelegates::OnWorldTickStart.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopEventType::PreWorldTick);
	FWorldDelegates::OnWorldPreActorTick.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopEventType::PreActorTick);
	FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopEventType::PostActorTick);
	FWorldDelegates::OnWorldTickEnd.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopEventType::PostWorldTick);
	
	ULevel* level = GetWorldRef().PersistentLevel;
	PrePhysicsTickFunction->RegisterTickFunction(level);
	DuringPhysicsTickFunction->RegisterTickFunction(level);
	PostPhysicsTickFunction->RegisterTickFunction(level);
	PostUpdateTickFunction->RegisterTickFunction(level);
}

void UZSharpEventLoopSubsystem::Deinitialize()
{
	PrePhysicsTickFunction->UnRegisterTickFunction();
	DuringPhysicsTickFunction->UnRegisterTickFunction();
	PostPhysicsTickFunction->UnRegisterTickFunction();
	PostUpdateTickFunction->UnRegisterTickFunction();
	
	Super::Deinitialize();
}

bool UZSharpEventLoopSubsystem::DoesSupportWorldType(const EWorldType::Type worldType) const
{
#if WITH_EDITOR
	return worldType == EWorldType::Game || worldType == EWorldType::PIE;
#else
	return worldType == EWorldType::Game;
#endif
}

void UZSharpEventLoopSubsystem::HandleWorldDelegate(UWorld* world, ELevelTick tickType, float, ZSharp::EZSharpEventLoopEventType eventType)
{
	if (world != GetWorld())
	{
		return;
	}

	NotifyEvent(eventType);
}

void UZSharpEventLoopSubsystem::NotifyEvent(ZSharp::EZSharpEventLoopEventType eventType)
{
	const FGameTime time = GetWorldRef().GetTime();
	ZSharp::FZSharpEventLoop_Interop::GNotifyEvent(eventType, time.GetDeltaWorldTimeSeconds(), time.GetDeltaRealTimeSeconds(), time.GetWorldTimeSeconds(), time.GetRealTimeSeconds());
}


