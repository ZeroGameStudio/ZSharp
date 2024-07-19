// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpEventLoopEventType.h"

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
class ZSHARPRUNTIME_API UZSharpEventLoopSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	friend FZSharpEventLoopTickFunction;

public:
	UZSharpEventLoopSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;
	
	virtual bool DoesSupportWorldType(const EWorldType::Type worldType) const override;

private:
	void HandleWorldDelegate(UWorld* world, ELevelTick, float, ZSharp::EZSharpEventLoopEventType eventType);
	void NotifyEvent(ZSharp::EZSharpEventLoopEventType eventType);

private:
	TUniquePtr<FZSharpEventLoopTickFunction> PrePhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> DuringPhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> PostPhysicsTickFunction;
	TUniquePtr<FZSharpEventLoopTickFunction> PostUpdateTickFunction;
	
};


