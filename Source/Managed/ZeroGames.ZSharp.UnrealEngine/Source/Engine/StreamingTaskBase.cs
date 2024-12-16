// Copyright Zero Games. All Rights Reserved.

using System.Runtime.ExceptionServices;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public abstract class StreamingTaskBase : IDisposable, IStreamingTask
{

	public enum EState : uint8
	{
		Loading = 0,
		Loaded = 1,
		Cancelled = 2,
	}
	
	public void Dispose()
	{
		Thrower.ThrowIfNotInGameThread();

		if (_state == EState.Loading)
		{
			throw new InvalidOperationException();
		}
		
		InternalDispose();
		
		GC.SuppressFinalize(this);
	}

	void IStreamingTask.Update(int32 loadedCount)
	{
		if (_lifecycle.IsExpired)
		{
			return;
		}
		
		InternalUpdate(loadedCount);
	}
	
	void IStreamingTask.SignalCompletion()
	{
		if (_lifecycle.IsExpired)
		{
			InternalCancel();
			return;
		}

		ensure(_state == EState.Loading);
		_state = EState.Loaded;
		
		if (_continuation is not null)
		{
			_continuation();
		}

		if (_additionalContinuations is not null)
		{
			foreach (var continuation in _additionalContinuations)
			{
				continuation();
			}
		}
	}
	
	public void ContinueWith(Action continuation)
	{
		GuardInvariant();
		
		if (_continuation is null)
		{
			_continuation = continuation;
		}
		else
		{
			_additionalContinuations ??= new();
			_additionalContinuations.Add(continuation);
		}
	}
	
	public bool IsCompleted
	{
		get
		{
			GuardInvariant();
			return _state > EState.Loading;
		}
	}

	internal StreamingTaskBase(IntPtr unmanaged, Lifecycle lifecycle)
	{
		_unmanaged = unmanaged;
		_lifecycle = lifecycle;
	}
	
	protected void GuardInvariant()
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (_unmanaged == default)
		{
			throw new InvalidOperationException();
		}
	}
	
	protected abstract void InternalUpdate(int32 loadedCount);

	protected void TryGetException()
	{
		if (_expiredException is null)
		{
			return;
		}
		
		_expiredException.Throw();
	}
	
	~StreamingTaskBase() => InternalDispose();

	private static unsafe void ReleaseUnmanaged(object? unmanaged) => StreamingTask_Interop.Release((IntPtr)unmanaged!);

	private void InternalCancel()
	{
		ensure(_state != EState.Loaded);
		_state = EState.Cancelled;
		_expiredException = ExceptionDispatchInfo.Capture(new LifecycleExpiredException(_lifecycle));
	}
	
	private void InternalDispose()
	{
		ensure(_state != EState.Loading);
		
		if (_unmanaged == default)
		{
			return;
		}

		IntPtr unmanaged = _unmanaged;
		_unmanaged = default;
		
		IGameThreadScheduler.Instance.Post(ReleaseUnmanaged, unmanaged);
	}
	
	protected IntPtr _unmanaged;
	private Lifecycle _lifecycle;
	private ExceptionDispatchInfo? _expiredException;

	private EState _state;

	private Action? _continuation;
	private List<Action>? _additionalContinuations;
	
}


