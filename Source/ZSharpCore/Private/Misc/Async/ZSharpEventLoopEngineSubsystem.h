// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZEventLoopTickingGroup.h"

#include "ZSharpEventLoopEngineSubsystem.generated.h"

UCLASS()
class UZSharpEventLoopEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	friend class UZSharpEventLoopWorldSubsystem;

public:
	UZSharpEventLoopEngineSubsystem();
	
public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	bool HandleTick(float deltaTime);
	void NotifyEvent(ZSharp::EZEventLoopTickingGroup group, float worldDeltaSeconds, float readDeltaSeconds, double worldElapsedSeconds, double realElapsedSeconds);

private:
	FTSTicker::FDelegateHandle TickDelegate;
	TBitArray<> TickingGroupMask;
	float RealtimeElapsedSeconds;

};


