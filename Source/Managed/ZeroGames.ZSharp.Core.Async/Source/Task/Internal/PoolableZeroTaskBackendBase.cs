// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal abstract class PoolableZeroTaskBackendBase<TResult, TImpl> : ZeroTaskBackendBase<TResult>, IPoolableZeroTaskBackend<TResult, TImpl> where TImpl : PoolableZeroTaskBackendBase<TResult, TImpl>, new()
{

	void IPoolableZeroTaskBackend<TResult, TImpl>.Initialize()
	{
		Comp.Initialize();
		Initialize();
	}

	void IPoolableZeroTaskBackend<TResult, TImpl>.Deinitialize()
	{
		Deinitialize();
		Lifecycle = default;
		Comp.Deinitialize();
	}
	
	TImpl? IPoolableZeroTaskBackend<TResult, TImpl>.PoolNext { get; set; }
	
	protected virtual void Initialize(){}
	protected virtual void Deinitialize(){}

	protected new void SetResult(TResult result)
	{
		try
		{
			base.SetResult(result);
		}
		finally
		{
			ReturnToPool();
		}
	}

	protected new void SetException(Exception exception)
	{
		try
		{
			base.SetException(exception);
		}
		finally
		{
			ReturnToPool();
		}
	}

	protected static ref ZeroTaskBackendPool<TResult, TImpl> Pool => ref _pool;

	private void ReturnToPool()
	{
		Comp.TryPublishUnobservedException();
		
		_pool.Push((TImpl)this);
	}
	
	private static ZeroTaskBackendPool<TResult, TImpl> _pool;

}