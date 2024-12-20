// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class ZeroStream
{
	
	public static IZeroStream<float> EveryTick(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle = default) => new ZeroStream_EveryTick(tickingGroup, lifecycle);
	public static IZeroStream<float> EveryTick(Lifecycle lifecycle = default) => EveryTick(EEventLoopTickingGroup.DuringWorldTimerTick, lifecycle);
	
}


