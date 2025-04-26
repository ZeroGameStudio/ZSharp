// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Pooling;

namespace ZeroGames.ZSharp.Core.Async;

internal abstract class PooledZeroTaskBackendBase<TResult, TImpl> : ZeroTaskBackendBase<TResult>, IReusedZeroTaskBackend, IPooled where TImpl : PooledZeroTaskBackendBase<TResult, TImpl>, new()
{

	void IPooled.PreGetFromPool()
	{
		GC.ReRegisterForFinalize(this);
		Initialize();
	}

	void IPooled.PreReturnToPool()
	{
		Deinitialize();
		Comp.TryPublishUnobservedException();
		Comp.Reset();
		Lifecycle = default;
	}
	
	protected static TImpl InternalGetFromPool() => _pool.Get();

	protected virtual void Initialize(){}
	protected virtual void Deinitialize(){}

	protected override void PostGetResult()
	{
		base.PostGetResult();
		ReturnToPool();
	}

	~PooledZeroTaskBackendBase() => ReturnToPool();

	private void ReturnToPool() => _pool.Return((TImpl)this);
	
	private static readonly ObjectPool<TImpl> _pool = new(new PoolingConfigProvider<TImpl>());

}