// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_DelayFrames : PoolableZeroTaskBackendBase<int32, ZeroTaskBackend_DelayFrames>
{

	public static ZeroTaskBackend_DelayFrames GetFromPool(int32 delayFrames, Lifecycle lifecycle, IProgress<int32>? progress)
	{
		var task = Pool.Pop();
		task._delayFrames = delayFrames;
		task.Lifecycle = lifecycle;

		return task;
	}

	public void Run()
	{
		_timer = GlobalTimerSchedulers.Frame.Register(static (deltaTime, state) =>
		{
			var @this = (ZeroTaskBackend_DelayFrames)state!;
			@this._expiredRegistration.Dispose();
			@this.SetResult(deltaTime);
		}, this, _delayFrames, false, false, Lifecycle, static (ex, state) =>
		{
			var @this = (ZeroTaskBackend_DelayFrames)state!;
			@this._expiredRegistration.Dispose();
			@this.SetException(ex);
		});
		
		// Timer doesn't care whether the Lifecycle is reactive or not so we do here.
		if (Lifecycle.TryToReactive(out var reactiveLifecycle))
		{
			reactiveLifecycle.RegisterOnExpired(static state =>
			{
				var (@this, token) = ((ZeroTaskBackend_DelayFrames, ZeroTaskToken))state!;
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
		_delayFrames = 0;
		_timer = default;
		_expiredRegistration = default;
		
		base.Deinitialize();
	}

	private int32 _delayFrames;
	
	private Timer _timer;
	private LifecycleExpiredRegistration _expiredRegistration;

}


