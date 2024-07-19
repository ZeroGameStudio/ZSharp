// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.EventLoop;

// WARNING: This class contains complex concurrent logic so do NOT modify if you don't know what you are doing!
internal class EventLoop : IEventLoop
{

	internal static EventLoop Get() => s_singleton;

	public EventLoopObserverHandle RegisterObserver(IEventLoopObserver observer)
	{
		EventLoopObserverHandle handle = AllocateHandleForObserver(observer);

		if (UnrealEngineStatics.IsInGameThread() && _isNotifing)
		{
			_deferredRequestMap[handle] = () => { InternalRegisterObserver(observer); };
			return handle;
		}
		else
		{
			return InternalRegisterWeakObserver(observer);
		}
	}

	public EventLoopObserverHandle RegisterWeakObserver(IEventLoopObserver observer)
	{
		EventLoopObserverHandle handle = AllocateHandleForObserver(observer);

		if (UnrealEngineStatics.IsInGameThread() && _isNotifing)
		{
			_deferredRequestMap[handle] = () => { InternalRegisterWeakObserver(observer); };
			return handle;
		}
		else
		{
			return InternalRegisterWeakObserver(observer);
		}
	}

	public EventLoopObserverHandle RegisterObserver(EEventLoopEventType eventType, EventLoopHandler observer)
	{
		EventLoopObserverHandle handle = AllocateHandle();

		if (UnrealEngineStatics.IsInGameThread() && _isNotifing)
		{
			_deferredRequestMap[handle] = () => { InternalRegisterObserver(handle, eventType, observer); };
			return handle;
		}
		else
		{
			return InternalRegisterObserver(handle, eventType, observer);
		}
	}

	public EventLoopObserverHandle RegisterObserver(EEventLoopEventType eventType, Action observer)
	{
		EventLoopObserverHandle handle = AllocateHandle();

		if (UnrealEngineStatics.IsInGameThread() && _isNotifing)
		{
			_deferredRequestMap[handle] = () => { InternalRegisterObserver(handle, eventType, observer); };
			return handle;
		}
		else
		{
			return InternalRegisterObserver(handle, eventType, observer);
		}
	}

	public EventLoopObserverHandle RegisterObserver(EEventLoopEventType eventType, Action<float> observer)
	{
		EventLoopObserverHandle handle = AllocateHandle();

		if (UnrealEngineStatics.IsInGameThread() && _isNotifing)
		{
			_deferredRequestMap[handle] = () => { InternalRegisterObserver(handle, eventType, observer); };
			return handle;
		}
		else
		{
			return InternalRegisterObserver(handle, eventType, observer);
		}
	}

	public void UnregisterObserver(IEventLoopObserver observer)
	{
		EventLoopObserverHandle handle;
		try
		{
			_handleLock.EnterReadLock();
			
			handle = observer.Handle;
			if (!handle.IsValid)
			{
				return;
			}
		}
		finally
		{
			_handleLock.ExitReadLock();
		}

		if (UnrealEngineStatics.IsInGameThread() && _isNotifing)
		{
			if (_deferredRequestMap.Remove(handle))
			{
				return;
			}
			
			_deferredRequestMap[handle] = () => { InternalUnregisterObserverInterface(handle); };
		}
		else
		{
			InternalUnregisterObserverInterface(handle);
		}
	}

	public void UnregisterObserver(EventLoopObserverHandle observer)
	{
		if (!observer.IsValid)
		{
			return;
		}
		
		if (UnrealEngineStatics.IsInGameThread() && _isNotifing)
		{
			if (_deferredRequestMap.Remove(observer))
			{
				return;
			}
			
			_deferredRequestMap[observer] = () => { InternalUnregisterObserver(observer); };
		}
		else
		{
			InternalUnregisterObserver(observer);
		}
	}

	internal void NotifyEvent(EEventLoopEventType eventType, float worldDeltaTime, float realDeltaTime, double worldElapsedTime, double realElapsedTime)
	{
		lock (_registryLock)
		{
			try
			{
				_worldAccumulatedTime += worldDeltaTime;
				_realAcccumulatedTime += realDeltaTime;
		
				EventLoopArgs args = new()
				{
					EventType = eventType,
					WorldDeltaTime = worldDeltaTime,
					RealDeltaTime = realDeltaTime,
					WorldElapsedTime = worldElapsedTime,
					RealElapsedTime = realElapsedTime,
					WorldAccumulatedTime = _worldAccumulatedTime,
					RealAccumulatedTime = _realAcccumulatedTime,
				};
		
				foreach (var pair in _interfaceObserverMap)
				{
					if (pair.Value.EventType == eventType)
					{
						pair.Value.NotifyEvent(args);
					}
				}
				
				foreach (var pair in _weakInterfaceObserverMap)
				{
					if (pair.Value.TryGetTarget(out var observer) && observer.EventType == eventType)
					{
						observer.NotifyEvent(args);
					}
					else if (_numStales < _stales.Length)
					{
						_stales[_numStales++] = pair.Key;
					}
				}

				foreach (var pair in _fullDelegateObserverMap)
				{
					if (pair.Value.Type == eventType)
					{
						pair.Value.Delegate.Invoke(args);
					}
				}

				foreach (var pair in _simpleDelegateObserverMap)
				{
					if (pair.Value.Type == eventType)
					{
						pair.Value.Delegate.Invoke();
					}
				}

				foreach (var pair in _worldDeltaDelegateObserverMap)
				{
					if (pair.Value.Type == eventType)
					{
						pair.Value.Delegate.Invoke(worldDeltaTime);
					}
				}
				
				ApplyDeferredRequests();
				Compact();
				
				_isNotifing = true;
			}
			finally
			{
				_isNotifing = false;
			}
		}
	}
	
	private EventLoopObserverHandle InternalRegisterObserver(IEventLoopObserver observer)
	{
		lock (_registryLock)
		{ 
			EventLoopObserverHandle handle = observer.Handle;
			_interfaceObserverMap[handle] = observer;
			return handle;
		}
	}

	private EventLoopObserverHandle InternalRegisterWeakObserver(IEventLoopObserver observer)
	{
		lock (_registryLock)
		{
			EventLoopObserverHandle handle = observer.Handle;
			_weakInterfaceObserverMap[handle] = new(observer);
			return handle;
		}
	}

	private EventLoopObserverHandle InternalRegisterObserver(EventLoopObserverHandle handle, EEventLoopEventType eventType, EventLoopHandler observer)
	{
		lock (_registryLock)
		{
			_fullDelegateObserverMap[handle] = (eventType, observer);
			return handle;
		}
	}

	private EventLoopObserverHandle InternalRegisterObserver(EventLoopObserverHandle handle, EEventLoopEventType eventType, Action observer)
	{
		lock (_registryLock)
		{
			_simpleDelegateObserverMap[handle] = (eventType, observer);
			return handle;
		}
	}

	private EventLoopObserverHandle InternalRegisterObserver(EventLoopObserverHandle handle, EEventLoopEventType eventType, Action<float> observer)
	{
		lock (_registryLock)
		{
			_worldDeltaDelegateObserverMap[handle] = (eventType, observer);
			return handle;
		}
	}

	private void InternalUnregisterObserverInterface(EventLoopObserverHandle observer)
	{
		lock (_registryLock)
		{
			if (_interfaceObserverMap.Remove(observer, out var observerObj))
			{
				ClearHandleForObserver(observerObj);
			}
		}
	}

	private void InternalUnregisterObserver(EventLoopObserverHandle observer)
	{
		lock (_registryLock)
		{
			if (_worldDeltaDelegateObserverMap.Remove(observer, out _))
			{
				return;
			}

			if (_simpleDelegateObserverMap.Remove(observer, out _))
			{
				return;
			}

			if (_fullDelegateObserverMap.Remove(observer, out _))
			{
				return;
			}

			if (_interfaceObserverMap.Remove(observer, out var observerObj))
			{
				ClearHandleForObserver(observerObj);
			}
		
			if (_weakInterfaceObserverMap.Remove(observer, out var observerWr))
			{
				if (observerWr.TryGetTarget(out observerObj))
				{
					ClearHandleForObserver(observerObj);
				}
			}
		}
	}

	private EventLoopObserverHandle AllocateHandle()
	{
		try
		{
			_handleLock.EnterWriteLock();
			
			return new(++_currentHandle);
		}
		finally
		{
			_handleLock.ExitWriteLock();
		}
	}

	private EventLoopObserverHandle AllocateHandleForObserver(IEventLoopObserver observer)
	{
		try
		{
			_handleLock.EnterWriteLock();
			
			if (observer.Handle.IsValid)
			{
				throw new InvalidOperationException();
			}
		
			EventLoopObserverHandle handle = new(++_currentHandle);
			observer.Handle = handle;

			return handle;
		}
		finally
		{
			_handleLock.ExitWriteLock();
		}
	}

	private void ClearHandleForObserver(IEventLoopObserver observer)
	{
		try
		{
			_handleLock.EnterWriteLock();
			
			observer.Handle = default;
		}
		finally
		{
			_handleLock.ExitWriteLock();
		}
	}
	
	private void ApplyDeferredRequests()
	{
		foreach (var pair in _deferredRequestMap)
		{
			pair.Value.Invoke();
		}
		
		_deferredRequestMap.Clear();
	}

	private void Compact()
	{
		for (int32 i = 0; i < _numStales; ++i)
		{
			_weakInterfaceObserverMap.Remove(_stales[i], out _);
		}
		
		_numStales = 0;
	}

	// This is pretty commonly used so create at startup.
	private static EventLoop s_singleton = new();

	// All codepaths that modify these containers have to acquire this lock.
	private object _registryLock = new();
	private Dictionary<EventLoopObserverHandle, IEventLoopObserver> _interfaceObserverMap = new();
	private Dictionary<EventLoopObserverHandle, WeakReference<IEventLoopObserver>> _weakInterfaceObserverMap = new();
	private Dictionary<EventLoopObserverHandle, (EEventLoopEventType Type, EventLoopHandler Delegate)> _fullDelegateObserverMap = new();
	private Dictionary<EventLoopObserverHandle, (EEventLoopEventType Type, Action Delegate)> _simpleDelegateObserverMap = new();
	private Dictionary<EventLoopObserverHandle, (EEventLoopEventType Type, Action<float> Delegate)> _worldDeltaDelegateObserverMap = new();
	
	// All codepaths that access _currentHandle or any IEventLoopObserver's Handle property have to acquire this lock.
	private ReaderWriterLockSlim _handleLock = new();
	private uint64 _currentHandle;
	
	// These two are only used by NotifyEvent() to clear stale weak observers.
	private EventLoopObserverHandle[] _stales = new EventLoopObserverHandle[4];
	private int32 _numStales;
	
	// These two are only used by GameThread to detect register/unregister during NotifyEvent().
	// By the way, other threads will be blocked until NotifyEvent() returns so they have no problem.
	private Dictionary<EventLoopObserverHandle, Action> _deferredRequestMap = new();
	private bool _isNotifing;
	
	private double _worldAccumulatedTime;
	private double _realAcccumulatedTime;

}


