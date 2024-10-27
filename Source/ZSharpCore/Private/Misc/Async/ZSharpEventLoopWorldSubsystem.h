// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZEventLoopTickingGroup.h"

#include "ZSharpEventLoopWorldSubsystem.generated.h"

class UZSharpEventLoopWorldSubsystem;

struct FZSharpEventLoopTickFunction final : public FTickFunction
{
	
	FZSharpEventLoopTickFunction(UZSharpEventLoopWorldSubsystem* owner, ETickingGroup group)
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
	UZSharpEventLoopWorldSubsystem* Owner;
	
};

UCLASS(MinimalAPI, Transient)
class UZSharpEventLoopWorldSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	friend FZSharpEventLoopTickFunction;

public:
	UZSharpEventLoopWorldSubsystem();

public:
	// USubsystem interface
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
	void HandleWorldDelegate(UWorld* world, ELevelTick, float, ZSharp::EZEventLoopTickingGroup group);
	void NotifyEvent(ZSharp::EZEventLoopTickingGroup group);
	void NotifyWorldTimerTick();

private:
	TUniquePtr<FZSharpEventLoopTickFunction> PrePhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> DuringPhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> PostPhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> PostUpdateTickFunction;
	
};


