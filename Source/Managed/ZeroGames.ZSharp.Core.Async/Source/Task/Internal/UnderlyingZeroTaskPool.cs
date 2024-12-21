// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal struct UnderlyingZeroTaskPool<TResult, TImpl> where TImpl : class, IPoolableUnderlyingZeroTask<TResult, TImpl>, new()
{
	
	public TImpl Pop()
	{
		TImpl task;
		if (_head is null)
		{
			task = new();
		}
		else
		{
			task = _head;
			_head = task.PoolNext;
		}
		
		task.Initialize();
		return task;
	}

	public void Push(TImpl task)
	{
		task.Deinitialize();
		
		if (_head is null)
		{
			_head = task;
		}
		else
		{
			task.PoolNext = _head;
			_head = task;
		}
	}

	private TImpl? _head;

}


