// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IPoolableLifecycleBackend<TImpl> : ILifecycleBackend where TImpl : class, IPoolableLifecycleBackend<TImpl>
{
	void Initialize();
	void Deinitialize();
	protected internal TImpl? PoolNext { get; set; }
}


