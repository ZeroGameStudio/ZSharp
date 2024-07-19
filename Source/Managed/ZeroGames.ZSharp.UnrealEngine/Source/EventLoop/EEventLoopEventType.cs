// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.EventLoop;

public enum EEventLoopEventType : uint8
{
	PreWorldTick,
	PreActorTick,
	PrePhysicsTick,
	DuringPhysicsTick,
	PostPhysicsTick,
	PostUpdateTick,
	PostActorTick,
	PostWorldTick,
}


