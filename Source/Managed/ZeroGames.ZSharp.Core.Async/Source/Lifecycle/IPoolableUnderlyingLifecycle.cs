// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public interface IPoolableUnderlyingLifecycle<TImpl> : IUnderlyingLifecycle where TImpl : class, IPoolableUnderlyingLifecycle<TImpl>
{
	static abstract TImpl Create();
	void Initialize();
	void Deinitialize();
	protected internal TImpl? PoolNext { get; set; }
}


