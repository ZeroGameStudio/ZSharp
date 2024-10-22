// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public enum EZeroTaskDelayType
{
	WorldPaused,
	WorldUnpaused,
	RealPaused,
	RealUnpaused,
}

internal class ZeroTask_Delay : UnderlyingZeroTaskBase<TimeSpan, ZeroTask_Delay>
{

	public static ZeroTask_Delay GetFromPool(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle, bool throwOnExpired)
	{
		ZeroTask_Delay task = Pool.Pop();
		task._delayType = delayType;
		task._delayTime = delayTime;
		task.Lifecycle = lifecycle;
		task.ShouldThrowOnLifecycleExpired = throwOnExpired;
		
		return task;
	}

	public void Run()
	{
		ITimerScheduler scheduler = _delayType switch
		{
			EZeroTaskDelayType.WorldPaused => GlobalTimerScheduler.WorldPausedReliable,
			EZeroTaskDelayType.WorldUnpaused => GlobalTimerScheduler.WorldUnpausedReliable,
			EZeroTaskDelayType.RealPaused => GlobalTimerScheduler.RealPausedReliable,
			EZeroTaskDelayType.RealUnpaused => GlobalTimerScheduler.RealUnpausedReliable,
			_ => GlobalTimerScheduler.WorldPausedReliable,
		};
		
		scheduler.Register(static (deltaTime, state) =>
		{
			ZeroTask_Delay @this = Unsafe.As<ZeroTask_Delay>(state!);
			@this.Comp.SetResult(deltaTime);
		}, this, _delayTime, false, Lifecycle, static (ex, state) => throw ex);
	}

	private EZeroTaskDelayType _delayType;
	private TimeSpan _delayTime;

}


