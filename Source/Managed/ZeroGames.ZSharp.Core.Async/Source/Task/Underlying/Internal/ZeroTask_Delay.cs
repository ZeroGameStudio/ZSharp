// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public enum EZeroTaskDelayType
{
	Paused,
	PausedUnreliable,
	Unpaused,
	Realtime,
}

internal class ZeroTask_Delay : UnderlyingZeroTaskBase<TimeSpan, ZeroTask_Delay>
{

	public static ZeroTask_Delay GetFromPool(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle, bool throwOnExpired)
	{
		ZeroTask_Delay task = Pool.Pop();
		task._delayType = delayType;
		task._delayTime = delayTime;
		task._delayTimer = default;
		task.Lifecycle = lifecycle;
		task.ShouldThrowOnLifecycleExpired = throwOnExpired;
		
		return task;
	}

	public void Run()
	{
		ITimerScheduler scheduler = _delayType switch
		{
			EZeroTaskDelayType.Paused => GlobalTimerScheduler.Paused,
			EZeroTaskDelayType.PausedUnreliable => GlobalTimerScheduler.PausedUnreliable,
			EZeroTaskDelayType.Unpaused => GlobalTimerScheduler.Unpaused,
			EZeroTaskDelayType.Realtime => GlobalTimerScheduler.Realtime,
			_ => GlobalTimerScheduler.Paused,
		};
		
		_delayTimer = scheduler.Register(static (deltaTime, state) =>
		{
			ZeroTask_Delay @this = Unsafe.As<ZeroTask_Delay>(state!);
			// This is to ensure the timer only triggers once.
			// This must call before SetResult because it will return the task to pool.
			@this._delayTimer.Unregister();
			@this.SetResult(deltaTime);
		}, this, _delayTime, false, Lifecycle, static (ex, state) =>
		{
			ZeroTask_Delay @this = Unsafe.As<ZeroTask_Delay>(state!);
			@this.SetException(ex);
		});
	}
	
	private EZeroTaskDelayType _delayType;
	private TimeSpan _delayTime;
	
	private Timer _delayTimer;

}


