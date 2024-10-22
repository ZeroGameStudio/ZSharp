// Copyright Zero Games. All Rights Reserved.


#include "ZSharpEventLoopSubsystem.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Interop/ZSharpEventLoop_Interop.h"

void FZSharpEventLoopTickFunction::Run() const
{
	static const TMap<ETickingGroup, ZSharp::EZSharpEventLoopTickingGroup> GTypeMap
	{
		{ TG_PrePhysics, ZSharp::EZSharpEventLoopTickingGroup::PrePhysicsTick },
		{ TG_DuringPhysics, ZSharp::EZSharpEventLoopTickingGroup::DuringPhysicsTick },
		{ TG_PostPhysics, ZSharp::EZSharpEventLoopTickingGroup::PostPhysicsTick },
		{ TG_PostUpdateWork, ZSharp::EZSharpEventLoopTickingGroup::PostUpdateTick },
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

	FWorldDelegates::OnWorldTickStart.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopTickingGroup::PreWorldTick);
	FWorldDelegates::OnWorldPreActorTick.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopTickingGroup::PreActorTick);
	FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopTickingGroup::PostActorTick);
	FWorldDelegates::OnWorldTickEnd.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZSharpEventLoopTickingGroup::PostWorldTick);

	const UWorld& world = GetWorldRef();
	world.GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::NotifyWorldTimerTick));
	
	ULevel* level = world.PersistentLevel;
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

	GetWorldRef().GetTimerManager().ClearAllTimersForObject(this);
	
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

void UZSharpEventLoopSubsystem::Tick(float DeltaTime)
{
	NotifyEvent(ZSharp::EZSharpEventLoopTickingGroup::PostWorldTimerTick);
}

TStatId UZSharpEventLoopSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UZSharpEventLoopSubsystem, STATGROUP_Tickables);
}

void UZSharpEventLoopSubsystem::HandleWorldDelegate(UWorld* world, ELevelTick, float, ZSharp::EZSharpEventLoopTickingGroup group)
{
	if (world != GetWorld())
	{
		return;
	}

	NotifyEvent(group);
}

void UZSharpEventLoopSubsystem::NotifyEvent(ZSharp::EZSharpEventLoopTickingGroup group)
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (!alc)
	{
		return;
	}

	alc->PushRedFrame();
	ON_SCOPE_EXIT { alc->PopRedFrame(); };
	
	const FGameTime time = GetWorldRef().GetTime();
	ZSharp::FZSharpEventLoop_Interop::GNotifyEvent(group, time.GetDeltaWorldTimeSeconds(), time.GetDeltaRealTimeSeconds(), time.GetWorldTimeSeconds(), time.GetRealTimeSeconds());
}

void UZSharpEventLoopSubsystem::NotifyWorldTimerTick()
{
	NotifyEvent(ZSharp::EZSharpEventLoopTickingGroup::DuringWorldTimerTick);
	GetWorldRef().GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::NotifyWorldTimerTick));
}


