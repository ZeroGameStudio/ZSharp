// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_CompletionSource<TResult> : PooledZeroTaskBackendBase<TResult, ZeroTaskBackend_CompletionSource<TResult>>
{
	
	public static ZeroTaskBackend_CompletionSource<TResult> GetFromPool(Lifecycle lifecycle)
	{
		var task = InternalGetFromPool();
		task.Lifecycle = lifecycle;

		return task;
	}
	
	public void SetResult(TResult result, ZeroTaskToken token)
	{
		// User may not know whether this is expired when using reactive lifecycle so we return silently.
		if (token != Token)
		{
			return;
		}

		if (_completed)
		{
			return;
		}

		_completed = true;

		if (Lifecycle.IsExpired)
		{
			SetException(new LifecycleExpiredException(Lifecycle));
		}
		else
		{
			SetResult(result);
		}
	}

	public void SetException(Exception exception, ZeroTaskToken token)
	{
		// User may not know whether this is expired when using reactive lifecycle so we return silently.
		if (token != Token)
		{
			return;
		}
		
		if (_completed)
		{
			return;
		}

		_completed = true;

		SetException(Lifecycle.IsExpired ? new LifecycleExpiredException(null, exception, Lifecycle) : exception);
	}

	public ZeroTask<TResult> GetTask(ZeroTaskToken token)
	{
		Comp.ValidateToken(token);
		return new(this);
	}

	protected override void Initialize()
	{
		base.Initialize();
		
		if (Lifecycle.TryToReactive(out var reactiveLifecycle))
		{
			reactiveLifecycle.RegisterOnExpired(static state =>
			{
				var (@this, token) = ((ZeroTaskBackend_CompletionSource<TResult>, ZeroTaskToken))state!;
				@this._completed = true;
				@this.SetException(new LifecycleExpiredException(@this.Lifecycle));
			}, (this, Token));
		}
	}

	protected override void Deinitialize()
	{
		_completed = false;
		base.Deinitialize();
	}
	
	private bool _completed;
	
}


