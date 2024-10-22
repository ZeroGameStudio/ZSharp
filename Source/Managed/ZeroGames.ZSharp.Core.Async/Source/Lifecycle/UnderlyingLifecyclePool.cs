// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public struct UnderlyingLifecyclePool<T> where T : class, IPoolableUnderlyingLifecycle<T>
{
	
	public T Pop()
	{
		if (_head is null)
		{
			return T.Create();
		}
		else
		{
			T lifecycle = _head;
			lifecycle.Initialize();
			_head = lifecycle.PoolNext;
			return lifecycle;
		}
	}

	public void Push(T lifecycle)
	{
		lifecycle.Deinitialize();
		
		if (_head is null)
		{
			_head = lifecycle;
		}
		else
		{
			lifecycle.PoolNext = _head;
			_head = lifecycle;
		}
	}

	private T? _head;

}