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
		Comp.ValidateToken(token);

		if (Lifecycle.IsExpired)
		{
			SetException(new LifecycleExpiredException(Lifecycle));
		}
		
		SetResult(result);
	}

	public void SetException(Exception exception, ZeroTaskToken token)
	{
		Comp.ValidateToken(token);
		
		if (Lifecycle.IsExpired)
		{
			SetException(new LifecycleExpiredException(null, exception, Lifecycle));
		}
		
		SetException(exception);
	}

	public ZeroTask<TResult> GetTask(ZeroTaskToken token)
	{
		Comp.ValidateToken(token);
		return new(this);
	}

}


