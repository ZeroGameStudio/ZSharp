// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class ZeroStream
{

	public static IZeroStream<float> Interval(EZeroTaskDelayType delayType, float intervalSeconds, Lifetime lifetime = default)
		=> new ZeroStream_Interval(delayType, intervalSeconds, lifetime);
	public static IZeroStream<float> Interval(float intervalSeconds, Lifetime lifetime = default)
		=> Interval(EZeroTaskDelayType.WorldPaused, intervalSeconds, lifetime);

	public static IZeroStream<float> Interval(EZeroTaskDelayType delayType, TimeSpan interval, Lifetime lifetime = default)
		=> Interval(delayType, (float)interval.TotalSeconds, lifetime);
	public static IZeroStream<float> Interval(TimeSpan interval, Lifetime lifetime = default)
		=> Interval(EZeroTaskDelayType.WorldPaused, interval, lifetime);
	
	public static IZeroStream<int32> IntervalFrame(int32 intervalFrames, Lifetime lifetime = default)
		=> new ZeroStream_IntervalFrame(intervalFrames, lifetime);

	public static IZeroStream<float> EveryTick(EEventLoopTickingGroup tickingGroup, Lifetime lifetime = default)
		=> new ZeroStream_EveryTick(tickingGroup, lifetime);
	public static IZeroStream<float> EveryTick(Lifetime lifetime = default)
		=> EveryTick(EEventLoopTickingGroup.DuringWorldTimerTick, lifetime);
	
}


