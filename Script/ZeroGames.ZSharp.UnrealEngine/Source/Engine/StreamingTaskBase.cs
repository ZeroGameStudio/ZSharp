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
		if (_lifetime.IsExpired)
		{
			return;
		}
		
		InternalUpdate(loadedCount);
	}
	
	void IStreamingTask.SignalCompletion()
	{
		if (_lifetime.IsExpired)
		{
			InternalCancel();
			return;
		}

		ensure(_state == EState.Loading);
		_state = EState.Loaded;
		
		_continuation.MoveNextSource?.MoveNext();
		_continuation.MoveNextDelegate?.Invoke();

		if (_additionalContinuations is not null)
		{
			foreach (var continuation in _additionalContinuations)
			{
				continuation.MoveNextSource?.MoveNext();
				continuation.MoveNextDelegate?.Invoke();
			}
		}
	}
	
	public void ContinueWith(Action continuation)
	{
		GuardInvariant();

		if (IsCompleted)
		{
			continuation();
			return;
		}
		
		if (_continuation == default)
		{
			_continuation = new(null, continuation);
		}
		else
		{
			_additionalContinuations ??= new();
			_additionalContinuations.Add(new(null, continuation));
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

	internal StreamingTaskBase(IntPtr unmanaged, Lifetime lifetime)
	{
		Unmanaged = unmanaged;
		_lifetime = lifetime;
	}

	private protected enum EEmptyConstructor;

	private protected StreamingTaskBase(EEmptyConstructor _)
	{
		_state = EState.Loaded;
	}
	
	protected void GuardInvariant()
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (Unmanaged == IntPtr.Zero)
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
	
	protected void ContinueWith(IMoveNextSource source)
	{
		GuardInvariant();
		
		if (IsCompleted)
		{
			source.MoveNext();
			return;
		}
		
		if (_continuation == default)
		{
			_continuation = new(source, null);
		}
		else
		{
			_additionalContinuations ??= new();
			_additionalContinuations.Add(new(source, null));
		}
	}
	
	protected IntPtr Unmanaged { get; private set; }

	private readonly record struct Continuation(IMoveNextSource? MoveNextSource, Action? MoveNextDelegate);
	
	~StreamingTaskBase() => InternalDispose();

	private void InternalCancel()
	{
		ensure(_state != EState.Loaded);
		_state = EState.Cancelled;
		_expiredException = ExceptionDispatchInfo.Capture(new LifetimeExpiredException(_lifetime));
	}
	
	private unsafe void InternalDispose()
	{
		ensure(_state != EState.Loading);
		
		if (Unmanaged == IntPtr.Zero)
		{
			return;
		}

		IntPtr unmanaged = Unmanaged;
		Unmanaged = IntPtr.Zero;

		if (IsInGameThread)
		{
			StreamingTask_Interop.Release(unmanaged);
		}
		else
		{
			IGameThreadScheduler.Instance.Post(state => StreamingTask_Interop.Release((IntPtr)state!), unmanaged);
		}
	}
	
	private readonly Lifetime _lifetime;
	private ExceptionDispatchInfo? _expiredException;

	private EState _state;

	private Continuation _continuation;
	private List<Continuation>? _additionalContinuations;
	
}


