// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpEventLoopTickingGroup.h"

#include "ZSharpEventLoopSubsystem.generated.h"

class UZSharpEventLoopSubsystem;

struct FZSharpEventLoopTickFunction : public FTickFunction
{
	
	FZSharpEventLoopTickFunction(UZSharpEventLoopSubsystem* owner, ETickingGroup group)
		: Owner(owner)
	{
		TickGroup = group;
		EndTickGroup = group;
		
		bCanEverTick = true;
		bStartWithTickEnabled = true;
		bAllowTickOnDedicatedServer = true;
		bHighPriority = true;
	}

	// FTickFunction interface
	virtual void ExecuteTick(float, ELevelTick, ENamedThreads::Type, const FGraphEventRef&) override { Run(); }
	virtual FString DiagnosticMessage() override { return TEXT("ZSharpEventLoopTickFunction"); }

	void Run() const;

private:
	UZSharpEventLoopSubsystem* Owner;
	
};

UCLASS()
class UZSharpEventLoopSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	friend FZSharpEventLoopTickFunction;

public:
	UZSharpEventLoopSubsystem();

public:
	// UWorldSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;
	
	virtual bool DoesSupportWorldType(const EWorldType::Type worldType) const override;

	// FTickableObjectBase interface
	virtual bool IsAllowedToTick() const override { return !HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject); }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	// FTickableGameObject interface
	virtual UWorld* GetTickableGameObjectWorld() const override { return &GetWorldRef(); }

private:
	void HandleWorldDelegate(UWorld* world, ELevelTick, float, ZSharp::EZSharpEventLoopTickingGroup group);
	void NotifyEvent(ZSharp::EZSharpEventLoopTickingGroup group);
	void NotifyWorldTimerTick();

private:
	TUniquePtr<FZSharpEventLoopTickFunction> PrePhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> DuringPhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> PostPhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> PostUpdateTickFunction;
	
};


