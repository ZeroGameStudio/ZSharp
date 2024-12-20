// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public enum EZeroTaskDelayType
{
	WorldPaused,
	WorldPausedUnreliable,
	WorldUnpaused,
	Realtime,
}

internal class ZeroTask_Delay : UnderlyingZeroTaskBase<float, ZeroTask_Delay>
{

	public static ZeroTask_Delay GetFromPool(EZeroTaskDelayType delayType, float delaySeconds, Lifecycle lifecycle)
	{
		ZeroTask_Delay task = Pool.Pop();
		task._delayType = delayType;
		task._delaySeconds = delaySeconds;
		task._delayTimer = default;
		task.Lifecycle = lifecycle;

		return task;
	}

	public void Run()
	{
		ITimerScheduler<float> scheduler = _delayType switch
		{
			EZeroTaskDelayType.WorldPaused => GlobalTimerSchedulers.WorldPaused,
			EZeroTaskDelayType.WorldPausedUnreliable => GlobalTimerSchedulers.WorldPausedUnreliable,
			EZeroTaskDelayType.WorldUnpaused => GlobalTimerSchedulers.WorldUnpaused,
			EZeroTaskDelayType.Realtime => GlobalTimerSchedulers.Realtime,
			_ => GlobalTimerSchedulers.WorldPaused,
		};
		
		_delayTimer = scheduler.Register(static (deltaTime, state) =>
		{
			ZeroTask_Delay @this = Unsafe.As<ZeroTask_Delay>(state!);
			@this.SetResult(deltaTime);
		}, this, _delaySeconds, false, false, Lifecycle, static (ex, state) =>
		{
			ZeroTask_Delay @this = Unsafe.As<ZeroTask_Delay>(state!);
			@this.SetException(ex);
		});
	}
	
	private EZeroTaskDelayType _delayType;
	private float _delaySeconds;
	
	private Timer _delayTimer;

}


