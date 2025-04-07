// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_DelaySeconds : PooledZeroTaskBackendBase<float, ZeroTaskBackend_DelaySeconds>
{

	public static ZeroTaskBackend_DelaySeconds GetFromPool(EZeroTaskDelayType delayType, float delaySeconds, Lifecycle lifecycle)
	{
		var task = InternalGetFromPool();
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
			_ => GlobalTimerSchedulers.WorldPaused
		};
		
		_timer = scheduler.Register(static (deltaTime, state) =>
		{
			var @this = (ZeroTaskBackend_DelaySeconds)state!;
			@this._expiredRegistration.Dispose();
			@this.SetResult(deltaTime);
		}, this, _delaySeconds, false, false, Lifecycle, static (ex, state) =>
		{
			var @this = (ZeroTaskBackend_DelaySeconds)state!;
			@this._expiredRegistration.Dispose();
			@this.SetException(ex);
		});

		// Timer doesn't care whether the Lifecycle is reactive or not so we do here.
		if (Lifecycle.TryToReactive(out var reactiveLifecycle))
		{
			reactiveLifecycle.RegisterOnExpired(static state =>
			{
				var (@this, token) = ((ZeroTaskBackend_DelaySeconds, ZeroTaskToken))state!;
				if (@this.IsCompletedEvenIfRecycled(token))
				{
					// Although we dispose expired registration in the timer callback,
					// This callback can even get invoked if lifecycle is backed by a CancellationToken owned by another thread.
					return;
				}
				@this._timer.Dispose();
				@this.SetException(new LifecycleExpiredException(@this.Lifecycle));
			}, (this, Token));
		}
	}
	
	protected override void Deinitialize()
	{
		_delayType = default;
		_delaySeconds = 0;
		_timer = default;
		_expiredRegistration = default;
		
		base.Deinitialize();
	}

	private EZeroTaskDelayType _delayType;
	private float _delaySeconds;
	
	private Timer _timer;
	private LifecycleExpiredRegistration _expiredRegistration;

}


