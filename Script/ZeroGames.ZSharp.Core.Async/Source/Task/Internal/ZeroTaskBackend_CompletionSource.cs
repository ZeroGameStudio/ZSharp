// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_CompletionSource<TResult> : PooledZeroTaskBackendBase<TResult, ZeroTaskBackend_CompletionSource<TResult>>
{
	
	public static ZeroTaskBackend_CompletionSource<TResult> GetFromPool(Lifetime lifetime)
	{
		var task = InternalGetFromPool();
		task.Lifetime = lifetime;

		return task;
	}
	
	public void SetResult(TResult result, ZeroTaskToken token)
	{
		// User may not know whether this is expired when using reactive lifetime so we return silently.
		if (token != Token)
		{
			return;
		}

		if (_completed)
		{
			return;
		}

		_completed = true;

		if (Lifetime.IsExpired)
		{
			SetException(new LifetimeExpiredException(Lifetime));
		}
		else
		{
			SetResult(result);
		}
	}

	public void SetException(Exception exception, ZeroTaskToken token)
	{
		// User may not know whether this is expired when using reactive lifetime so we return silently.
		if (token != Token)
		{
			return;
		}
		
		if (_completed)
		{
			return;
		}

		_completed = true;

		SetException(Lifetime.IsExpired ? new LifetimeExpiredException(null, exception, Lifetime) : exception);
	}

	public ZeroTask<TResult> GetTask(ZeroTaskToken token)
	{
		Comp.ValidateToken(token);
		return new(this);
	}

	protected override void Initialize()
	{
		base.Initialize();
		
		if (Lifetime.TryToReactive(out var reactiveLifetime))
		{
			reactiveLifetime.RegisterOnExpired(static state =>
			{
				var (@this, token) = ((ZeroTaskBackend_CompletionSource<TResult>, ZeroTaskToken))state!;
				@this._completed = true;
				@this.SetException(new LifetimeExpiredException(@this.Lifetime));
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


