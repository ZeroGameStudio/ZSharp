// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_DelaySeconds : PooledZeroTaskBackendBase<float, ZeroTaskBackend_DelaySeconds>
{

	public static ZeroTaskBackend_DelaySeconds GetFromPool(EZeroTaskDelayType delayType, float delaySeconds, Lifetime lifetime)
	{
		var task = InternalGetFromPool();
		task._delayType = delayType;
		task._delaySeconds = delaySeconds;
		task.Lifetime = lifetime;

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
		}, this, _delaySeconds, false, false, Lifetime, static (ex, state) =>
		{
			var @this = (ZeroTaskBackend_DelaySeconds)state!;
			@this._expiredRegistration.Dispose();
			@this.SetException(ex);
		});

		// Timer doesn't care whether the Lifetime is reactive or not so we do here.
		if (Lifetime.TryToReactive(out var reactiveLifetime))
		{
			reactiveLifetime.RegisterOnExpired(static state =>
			{
				var (@this, token) = ((ZeroTaskBackend_DelaySeconds, ZeroTaskToken))state!;
				if (@this.IsCompletedEvenIfRecycled(token))
				{
					// Although we dispose expired registration in the timer callback,
					// This callback can even get invoked if lifetime is backed by a CancellationToken owned by another thread.
					return;
				}
				@this._timer.Dispose();
				@this.SetException(new LifetimeExpiredException(@this.Lifetime));
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
	private LifetimeExpiredRegistration _expiredRegistration;

}


