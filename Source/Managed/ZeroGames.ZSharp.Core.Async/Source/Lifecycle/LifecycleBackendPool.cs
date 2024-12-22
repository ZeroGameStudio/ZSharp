// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public struct LifecycleBackendPool<T> where T : class, IPoolableLifecycleBackend<T>, new()
{
	
	public T Pop()
	{
		T lifecycle = _head ?? new();
		_head = lifecycle.PoolNext;
		lifecycle.PoolNext = null;
		
		lifecycle.Initialize();
		return lifecycle;
	}

	public void Push(T lifecycle)
	{
		lifecycle.Deinitialize();
		
		lifecycle.PoolNext = _head;
		_head = lifecycle;
	}

	private T? _head;

}