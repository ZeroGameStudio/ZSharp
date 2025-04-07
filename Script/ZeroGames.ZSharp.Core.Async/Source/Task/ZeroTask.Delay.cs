// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{
	
	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();

		if (lifecycle.IsExpired)
		{
			return FromExpired<float>(lifecycle);
		}

		var backend = ZeroTaskBackend_DelaySeconds.GetFromPool(delayType, delayTime.Seconds, lifecycle);
		ZeroTask<float> task = FromBackend(backend);
		backend.Run();
		return task;
	}
	public static ZeroTask<float> Delay(TimeSpan delayTime, Lifecycle lifecycle = default)
		=> Delay(EZeroTaskDelayType.WorldPaused, delayTime, lifecycle);
	
	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, float delaySeconds, Lifecycle lifecycle = default)
		=> Delay(delayType, TimeSpan.FromSeconds(delaySeconds), lifecycle);
	public static ZeroTask<float> Delay(float delaySeconds, Lifecycle lifecycle = default)
		=> Delay(EZeroTaskDelayType.WorldPaused, delaySeconds, lifecycle);

	public static ZeroTask<int32> DelayFrame(int32 delayFrames, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (lifecycle.IsExpired)
		{
			return FromExpired<int32>(lifecycle);
		}

		var backend = ZeroTaskBackend_DelayFrames.GetFromPool(delayFrames, lifecycle);
		ZeroTask<int32> task = FromBackend(backend);
		backend.Run();
		return task;
	}

	public static ZeroTask<float> Yield(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (lifecycle.IsExpired)
		{
			return FromExpired<float>(lifecycle);
		}
		
		var backend = ZeroTaskBackend_Yield.GetFromPool(tickingGroup, lifecycle);
		ZeroTask<float> task = FromBackend(backend);
		backend.Run();
		return task;
	}
	public static ZeroTask<float> Yield(Lifecycle lifecycle = default) => Yield(EEventLoopTickingGroup.DuringWorldTimerTick, lifecycle);

}


