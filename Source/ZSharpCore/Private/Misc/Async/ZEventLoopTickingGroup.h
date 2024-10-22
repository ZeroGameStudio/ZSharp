// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	enum class EZEventLoopTickingGroup : uint8
	{
		PreWorldTick,
		PreActorTick,
		PrePhysicsTick,
		DuringPhysicsTick,
		PostPhysicsTick,
		DuringWorldTimerTick,
		PostWorldTimerTick,
		PostUpdateTick,
		PostActorTick,
		PostWorldTick,
	};
}


