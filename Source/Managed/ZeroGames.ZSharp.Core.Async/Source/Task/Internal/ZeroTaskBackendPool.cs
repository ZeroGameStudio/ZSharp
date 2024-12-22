// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal struct ZeroTaskBackendPool<TResult, TImpl> where TImpl : class, IPoolableZeroTaskBackend<TResult, TImpl>, new()
{
	
	public TImpl Pop()
	{
		TImpl task = _head ?? new();
		_head = task.PoolNext;
		task.PoolNext = null;
		
		task.Initialize();
		return task;
	}

	public void Push(TImpl task)
	{
		task.Deinitialize();
		
		task.PoolNext = _head;
		_head = task;
	}

	private TImpl? _head;

}


