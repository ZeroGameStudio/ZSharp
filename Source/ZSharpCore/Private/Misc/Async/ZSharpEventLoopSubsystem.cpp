// Copyright Zero Games. All Rights Reserved.


#include "ZSharpEventLoopSubsystem.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Interop/ZEventLoop_Interop.h"

void FZSharpEventLoopTickFunction::Run() const
{
	static const TMap<ETickingGroup, ZSharp::EZEventLoopTickingGroup> GTypeMap
	{
		{ TG_PrePhysics, ZSharp::EZEventLoopTickingGroup::PrePhysicsTick },
		{ TG_DuringPhysics, ZSharp::EZEventLoopTickingGroup::DuringPhysicsTick },
		{ TG_PostPhysics, ZSharp::EZEventLoopTickingGroup::PostPhysicsTick },
		{ TG_PostUpdateWork, ZSharp::EZEventLoopTickingGroup::PostUpdateTick },
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

	FWorldDelegates::OnWorldTickStart.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZEventLoopTickingGroup::PreWorldTick);
	FWorldDelegates::OnWorldPreActorTick.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZEventLoopTickingGroup::PreActorTick);
	FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZEventLoopTickingGroup::PostActorTick);
	FWorldDelegates::OnWorldTickEnd.AddUObject(this, &ThisClass::HandleWorldDelegate, ZSharp::EZEventLoopTickingGroup::PostWorldTick);

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
	NotifyEvent(ZSharp::EZEventLoopTickingGroup::PostWorldTimerTick);
}

TStatId UZSharpEventLoopSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UZSharpEventLoopSubsystem, STATGROUP_Tickables);
}

void UZSharpEventLoopSubsystem::HandleWorldDelegate(UWorld* world, ELevelTick, float, ZSharp::EZEventLoopTickingGroup group)
{
	if (world != GetWorld())
	{
		return;
	}

	NotifyEvent(group);
}

void UZSharpEventLoopSubsystem::NotifyEvent(ZSharp::EZEventLoopTickingGroup group)
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc();
	if (!alc)
	{
		return;
	}

	alc->PushRedFrame();
	ON_SCOPE_EXIT { alc->PopRedFrame(); };
	
	const FGameTime time = GetWorldRef().GetTime();
	ZSharp::FZEventLoop_Interop::GNotifyEvent(group, time.GetDeltaWorldTimeSeconds(), time.GetDeltaRealTimeSeconds(), time.GetWorldTimeSeconds(), time.GetRealTimeSeconds());
}

void UZSharpEventLoopSubsystem::NotifyWorldTimerTick()
{
	NotifyEvent(ZSharp::EZEventLoopTickingGroup::DuringWorldTimerTick);
	GetWorldRef().GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::NotifyWorldTimerTick));
}


