// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IPoolableUnderlyingLifecycle<TImpl> : IUnderlyingLifecycle where TImpl : class, IPoolableUnderlyingLifecycle<TImpl>
{
	void Initialize();
	void Deinitialize();
	protected internal TImpl? PoolNext { get; set; }
}


