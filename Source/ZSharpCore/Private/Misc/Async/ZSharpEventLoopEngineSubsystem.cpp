﻿// Copyright Zero Games. All Rights Reserved.

#include "ZSharpEventLoopEngineSubsystem.h"

#include "ZSharpCoreLogChannels.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRedFrameScope.h"
#include "CLR/ZSharpClr.h"
#include "Interop/Async/ZEventLoop_Interop.h"

UZSharpEventLoopEngineSubsystem::UZSharpEventLoopEngineSubsystem()
	: TickingGroupMask(false, static_cast<int32>(ZSharp::EZEventLoopTickingGroup::Num))
	, RealtimeElapsedSeconds(0.f)
{
	
}

void UZSharpEventLoopEngineSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	TickDelegate = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ThisClass::HandleTick));
}

void UZSharpEventLoopEngineSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegate);
	
	Super::Deinitialize();
}

bool UZSharpEventLoopEngineSubsystem::HandleTick(float deltaTime)
{
	RealtimeElapsedSeconds += deltaTime;
	NotifyEvent(ZSharp::EZEventLoopTickingGroup::RealtimeTick, 0.f, deltaTime, 0, RealtimeElapsedSeconds);
	for (int32 i = 0; i < TickingGroupMask.Num(); ++i)
	{
		TickingGroupMask[i] = false;
	}

	return true;
}

void UZSharpEventLoopEngineSubsystem::NotifyEvent(ZSharp::EZEventLoopTickingGroup group, float worldDeltaSeconds, float readDeltaSeconds, double worldElapsedSeconds, double realElapsedSeconds)
{
	if (!ensure(!TickingGroupMask[static_cast<int32>(group)]))
	{
		return;
	}

	ZSharp::FZRedFrameScope scope;
	{
#if !UE_BUILD_SHIPPING
		FString fatalMessage;
		ZSharp::FZEventLoop_Interop::GNotifyEvent(group, worldDeltaSeconds, readDeltaSeconds, worldElapsedSeconds, realElapsedSeconds, &fatalMessage);
		if (!fatalMessage.IsEmpty())
		{
			UE_LOG(LogZSharpCoreAsync, Fatal, TEXT("%s"), *fatalMessage);
		}
#else
		ZSharp::FZEventLoop_Interop::GNotifyEvent(group, worldDeltaSeconds, readDeltaSeconds, worldElapsedSeconds, realElapsedSeconds, nullptr);
#endif
	}
}


