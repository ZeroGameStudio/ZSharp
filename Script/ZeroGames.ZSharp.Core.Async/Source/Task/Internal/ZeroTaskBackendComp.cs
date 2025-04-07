// Copyright Zero Games. All Rights Reserved.

using System.Runtime.ExceptionServices;

namespace ZeroGames.ZSharp.Core.Async;

/// <summary>
/// Encapsulates generic logic for task backend.
/// Similar to ManualResetValueTaskSourceCore.
/// </summary>
internal struct ZeroTaskBackendComp<TResult>
{

	public void Initialize()
	{
		if (Token == default)
		{
			// First instantiate, move to a valid token.
			Token = Token.Next;
		}
	}

	public void Deinitialize()
	{
		// Invalidate token immediately.
		Token = Token.Next;
		
		_completed = false;
		_resultGot = false;
		
		_moveNextSource = null;
		_continuation = null;
		_error = null;
	}

	public EZeroTaskStatus GetStatus(ZeroTaskToken token)
	{
		ValidateToken(token);

		if (!_completed || (_moveNextSource is null && _continuation is null))
		{
			return EZeroTaskStatus.Pending;
		}

		if (_error is null)
		{
			return EZeroTaskStatus.Succeeded;
		}

		return _error.SourceException is OperationCanceledException ? EZeroTaskStatus.Canceled : EZeroTaskStatus.Faulted;
	}

	public TResult GetResult(ZeroTaskToken token)
	{
		ValidateToken(token);

		if (!_completed)
		{
			throw new InvalidOperationException("ZeroTask only supports await.");
		}

		_resultGot = true;
		_error?.Throw();
		return _result;
	}
	
	public void SetMoveNextSource(IMoveNextSource source, ZeroTaskToken token)
	{
		ValidateToken(token);
		ValidateContinuation();
		_moveNextSource = source;
	}

	public void SetContinuation(Action continuation, ZeroTaskToken token)
	{
		ValidateToken(token);
		ValidateContinuation();
		_continuation = continuation;
	}

	public void SetResult(TResult result)
	{
		_result = result;
		SignalCompletion();
	}

	public void SetException(Exception exception)
	{
		_error = ExceptionDispatchInfo.Capture(exception);
		SignalCompletion();
	}
	
	public void TryPublishUnobservedException()
	{
		if (_completed && !_resultGot)
		{
			_resultGot = true;
			if (_error?.SourceException is { } ex)
			{
				UnhandledExceptionHelper.Guard(ex, "Unobserved exception detected in ZeroTask.");
			}
		}
	}
	
	public void ValidateToken(ZeroTaskToken token)
	{
		if (token != Token)
		{
			throw new InvalidOperationException("Token expired.");
		}
	}
	
	public ZeroTaskToken Token { get; private set; }

	private void ValidateContinuation()
	{
		if (_moveNextSource is not null || _continuation is not null)
		{
			throw new InvalidOperationException("Await one instance of ZeroTask more than once.");
		}
	}

	private void SignalCompletion()
	{
		check(!_completed);
		
		_completed = true;

		if (_moveNextSource is not null)
		{
			_moveNextSource.MoveNext();
		}
		else
		{
			_continuation?.Invoke();
		}
	}

	private bool _completed;
	private bool _resultGot;
	
	private IMoveNextSource? _moveNextSource;
	private Action? _continuation;
	private TResult _result;
	private ExceptionDispatchInfo? _error;

}