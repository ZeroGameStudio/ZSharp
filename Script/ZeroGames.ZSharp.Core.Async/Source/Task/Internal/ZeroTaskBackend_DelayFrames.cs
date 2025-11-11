// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_DelayFrames : PooledZeroTaskBackendBase<int32, ZeroTaskBackend_DelayFrames>
{

	public static ZeroTaskBackend_DelayFrames GetFromPool(int32 delayFrames, Lifetime lifetime)
	{
		var task = InternalGetFromPool();
		task._delayFrames = delayFrames;
		task.Lifetime = lifetime;

		return task;
	}

	public void Run()
	{
		_timer = GlobalTimerSchedulers.Frame.Register(static (deltaTime, state) =>
		{
			var @this = (ZeroTaskBackend_DelayFrames)state!;
			@this._expiredRegistration.Dispose();
			@this.SetResult(deltaTime);
		}, this, _delayFrames, false, false, Lifetime, static (ex, state) =>
		{
			var @this = (ZeroTaskBackend_DelayFrames)state!;
			@this._expiredRegistration.Dispose();
			@this.SetException(ex);
		});
		
		// Timer doesn't care whether the Lifetime is reactive or not so we do here.
		if (Lifetime.TryToReactive(out var reactiveLifetime))
		{
			reactiveLifetime.RegisterOnExpired(static state =>
			{
				var (@this, token) = ((ZeroTaskBackend_DelayFrames, ZeroTaskToken))state!;
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
		_delayFrames = 0;
		_timer = default;
		_expiredRegistration = default;
		
		base.Deinitialize();
	}

	private int32 _delayFrames;
	
	private Timer _timer;
	private LifetimeExpiredRegistration _expiredRegistration;

}


