// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{
	
	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifetime lifetime = default)
	{
		Thrower.ThrowIfNotInGameThread();

		if (lifetime.IsExpired)
		{
			return FromExpired<float>(lifetime);
		}

		var backend = ZeroTaskBackend_DelaySeconds.GetFromPool(delayType, (float)delayTime.TotalSeconds, lifetime);
		ZeroTask<float> task = FromBackend(backend);
		backend.Run();
		return task;
	}
	public static ZeroTask<float> Delay(TimeSpan delayTime, Lifetime lifetime = default)
		=> Delay(EZeroTaskDelayType.WorldPaused, delayTime, lifetime);
	
	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, float delaySeconds, Lifetime lifetime = default)
		=> Delay(delayType, TimeSpan.FromSeconds(delaySeconds), lifetime);
	public static ZeroTask<float> Delay(float delaySeconds, Lifetime lifetime = default)
		=> Delay(EZeroTaskDelayType.WorldPaused, delaySeconds, lifetime);
	public static ZeroTask<float> Delay(double delaySeconds, Lifetime lifetime = default)
		=> Delay(EZeroTaskDelayType.WorldPaused, (float)delaySeconds, lifetime);

	public static ZeroTask<int32> DelayFrame(int32 delayFrames, Lifetime lifetime = default)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (lifetime.IsExpired)
		{
			return FromExpired<int32>(lifetime);
		}

		var backend = ZeroTaskBackend_DelayFrames.GetFromPool(delayFrames, lifetime);
		ZeroTask<int32> task = FromBackend(backend);
		backend.Run();
		return task;
	}

	public static ZeroTask<float> Yield(EEventLoopTickingGroup tickingGroup, Lifetime lifetime = default)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (lifetime.IsExpired)
		{
			return FromExpired<float>(lifetime);
		}
		
		var backend = ZeroTaskBackend_Yield.GetFromPool(tickingGroup, lifetime);
		ZeroTask<float> task = FromBackend(backend);
		backend.Run();
		return task;
	}
	public static ZeroTask<float> Yield(Lifetime lifetime = default) => Yield(EEventLoopTickingGroup.DuringWorldTimerTick, lifetime);

}


