// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public enum EZeroTaskDelayType
{
	WorldPaused,
	WorldUnpaused,
	Realtime,
}

internal class ZeroTaskBackend_DelaySeconds : PoolableZeroTaskBackendBase<float, ZeroTaskBackend_DelaySeconds>
{

	public static ZeroTaskBackend_DelaySeconds GetFromPool(EZeroTaskDelayType delayType, float delaySeconds, Lifecycle lifecycle, IProgress<float>? progress)
	{
		var task = Pool.Pop();
		task._delayType = delayType;
		task._delaySeconds = delaySeconds;
		task.Lifecycle = lifecycle;

		return task;
	}

	public void Run()
	{
		ITimerScheduler<float> scheduler = _delayType switch
		{
			EZeroTaskDelayType.WorldPaused => GlobalTimerSchedulers.WorldPaused,
			EZeroTaskDelayType.WorldUnpaused => GlobalTimerSchedulers.WorldUnpaused,
			EZeroTaskDelayType.Realtime => GlobalTimerSchedulers.Realtime,
			_ => GlobalTimerSchedulers.WorldPaused,
		};
		
		_timer = scheduler.Register(static (deltaTime, state) =>
		{
			ZeroTaskBackend_DelaySeconds @this = Unsafe.As<ZeroTaskBackend_DelaySeconds>(state!);
			@this.SetResult(deltaTime);
		}, this, _delaySeconds, false, false, Lifecycle, static (ex, state) =>
		{
			ZeroTaskBackend_DelaySeconds @this = Unsafe.As<ZeroTaskBackend_DelaySeconds>(state!);
			@this.SetException(ex);
		});
	}
	
	protected override void Deinitialize()
	{
		_delayType = default;
		_delaySeconds = 0;
		_timer = default;
		
		base.Deinitialize();
	}
	
	private EZeroTaskDelayType _delayType;
	private float _delaySeconds;
	
	private Timer _timer;

}


