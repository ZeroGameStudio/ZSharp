// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ZeroTask
{
	
	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle = default, IProgress<float>? progress = null)
	{
		Thrower.ThrowIfNotInGameThread();

		var underlyingTask = ZeroTask_DelaySeconds.GetFromPool(delayType, delayTime.Seconds, lifecycle, progress);
		ZeroTask<float> task = FromUnderlyingTask(underlyingTask);
		underlyingTask.Run();
		return task;
	}
	public static ZeroTask<float> Delay(TimeSpan delayTime, Lifecycle lifecycle = default, IProgress<float>? progress = null)
		=> Delay(EZeroTaskDelayType.WorldPaused, delayTime, lifecycle);
	
	public static ZeroTask<float> DelaySeconds(EZeroTaskDelayType delayType, float delaySeconds, Lifecycle lifecycle = default, IProgress<float>? progress = null)
		=> Delay(delayType, TimeSpan.FromSeconds(delaySeconds), lifecycle, progress);
	public static ZeroTask<float> DelaySeconds(float delaySeconds, Lifecycle lifecycle = default, IProgress<float>? progress = null)
		=> DelaySeconds(EZeroTaskDelayType.WorldPaused, delaySeconds, lifecycle);

	public static ZeroTask<int32> DelayFrames(int32 delayFrames, Lifecycle lifecycle = default, IProgress<int32>? progress = null)
	{
		Thrower.ThrowIfNotInGameThread();

		var underlyingTask = ZeroTask_DelayFrames.GetFromPool(delayFrames, lifecycle, progress);
		ZeroTask<int32> task = FromUnderlyingTask(underlyingTask);
		underlyingTask.Run();
		return task;
	}

	public static ZeroTask<float> Yield(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();
		
		var underlyingTask = ZeroTask_Yield.GetFromPool(tickingGroup, lifecycle);
		ZeroTask<float> task = FromUnderlyingTask(underlyingTask);
		underlyingTask.Run();
		return task;
	}
	
}


