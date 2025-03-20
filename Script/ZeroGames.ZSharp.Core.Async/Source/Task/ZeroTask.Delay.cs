// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ZeroTask
{
	
	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle = default, IProgress<float>? progress = null)
	{
		Thrower.ThrowIfNotInGameThread();

		if (lifecycle.IsExpired)
		{
			return FromExpired<float>(lifecycle);
		}

		var backend = ZeroTaskBackend_DelaySeconds.GetFromPool(delayType, delayTime.Seconds, lifecycle, progress);
		ZeroTask<float> task = FromBackend(backend);
		backend.Run();
		return task;
	}
	public static ZeroTask<float> Delay(TimeSpan delayTime, Lifecycle lifecycle = default, IProgress<float>? progress = null)
		=> Delay(EZeroTaskDelayType.WorldPaused, delayTime, lifecycle, progress);
	
	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, float delaySeconds, Lifecycle lifecycle = default, IProgress<float>? progress = null)
		=> Delay(delayType, TimeSpan.FromSeconds(delaySeconds), lifecycle, progress);
	public static ZeroTask<float> Delay(float delaySeconds, Lifecycle lifecycle = default, IProgress<float>? progress = null)
		=> Delay(EZeroTaskDelayType.WorldPaused, delaySeconds, lifecycle, progress);

	public static ZeroTask<int32> DelayFrame(int32 delayFrames, Lifecycle lifecycle = default, IProgress<int32>? progress = null)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (lifecycle.IsExpired)
		{
			return FromExpired<int32>(lifecycle);
		}

		var backend = ZeroTaskBackend_DelayFrames.GetFromPool(delayFrames, lifecycle, progress);
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


