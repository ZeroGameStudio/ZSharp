// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	enum class EZSharpEventLoopEventType : uint8
	{
		PreWorldTick,
		PreActorTick,
		PrePhysicsTick,
		DuringPhysicsTick,
		PostPhysicsTick,
		PostUpdateTick,
		PostActorTick,
		PostWorldTick,
	};
}


