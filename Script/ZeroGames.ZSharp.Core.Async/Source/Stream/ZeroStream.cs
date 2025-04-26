// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class ZeroStream
{

	public static IZeroStream<float> Interval(EZeroTaskDelayType delayType, float intervalSeconds, Lifecycle lifecycle = default)
		=> new ZeroStream_Interval(delayType, intervalSeconds, lifecycle);
	public static IZeroStream<float> Interval(float intervalSeconds, Lifecycle lifecycle = default)
		=> Interval(EZeroTaskDelayType.WorldPaused, intervalSeconds, lifecycle);

	public static IZeroStream<float> Interval(EZeroTaskDelayType delayType, TimeSpan interval, Lifecycle lifecycle = default)
		=> Interval(delayType, (float)interval.TotalSeconds, lifecycle);
	public static IZeroStream<float> Interval(TimeSpan interval, Lifecycle lifecycle = default)
		=> Interval(EZeroTaskDelayType.WorldPaused, interval, lifecycle);
	
	public static IZeroStream<int32> IntervalFrame(int32 intervalFrames, Lifecycle lifecycle = default)
		=> new ZeroStream_IntervalFrame(intervalFrames, lifecycle);

	public static IZeroStream<float> EveryTick(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle = default)
		=> new ZeroStream_EveryTick(tickingGroup, lifecycle);
	public static IZeroStream<float> EveryTick(Lifecycle lifecycle = default)
		=> EveryTick(EEventLoopTickingGroup.DuringWorldTimerTick, lifecycle);
	
}


