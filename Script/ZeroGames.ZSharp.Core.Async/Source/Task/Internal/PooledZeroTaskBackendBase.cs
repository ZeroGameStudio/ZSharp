// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Pooling;

namespace ZeroGames.ZSharp.Core.Async;

internal abstract class PooledZeroTaskBackendBase<TResult, TImpl> : ZeroTaskBackendBase<TResult>, IPooled where TImpl : PooledZeroTaskBackendBase<TResult, TImpl>, new()
{

	void IPooled.PreGetFromPool()
	{
		Comp.Initialize();
		Initialize();
	}

	void IPooled.PreReturnToPool()
	{
		Deinitialize();
		Lifecycle = default;
		Comp.Deinitialize();
	}
	
	protected static TImpl InternalGetFromPool() => _pool.Get();
	
	protected virtual void Initialize(){}
	protected virtual void Deinitialize()
	{
		Comp.TryPublishUnobservedException();
	}

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

	private void ReturnToPool() => _pool.Return((TImpl)this);
	
	private static readonly ObjectPool<TImpl> _pool = new(new PoolingConfigProvider<TImpl>());

}