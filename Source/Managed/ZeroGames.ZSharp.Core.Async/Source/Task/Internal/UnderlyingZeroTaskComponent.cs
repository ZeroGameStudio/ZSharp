// Copyright Zero Games. All Rights Reserved.

using System.Runtime.ExceptionServices;

namespace ZeroGames.ZSharp.Core.Async;

/// <summary>
/// Encapsulates generic logic for underlying task.
/// Similar to ManualResetValueTaskSourceCore.
/// </summary>
internal struct UnderlyingZeroTaskComponent<TResult>
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
		_moveNextSource = null;
		_continuation = null;
		_error = null;
		ResultGot = false;
	}

	public EUnderlyingZeroTaskStatus GetStatus(UnderlyingZeroTaskToken token)
	{
		ValidateToken(token);

		if (!_completed || (_moveNextSource is null && _continuation is null))
		{
			return EUnderlyingZeroTaskStatus.Pending;
		}

		if (_error is null)
		{
			return EUnderlyingZeroTaskStatus.Succeeded;
		}

		return _error.SourceException is OperationCanceledException ? EUnderlyingZeroTaskStatus.Canceled : EUnderlyingZeroTaskStatus.Faulted;
	}

	public TResult GetResult(UnderlyingZeroTaskToken token)
	{
		ValidateToken(token);

		if (!_completed)
		{
			throw new InvalidOperationException("ZeroTask only supports await.");
		}

		ResultGot = true;
		_error?.Throw();
		return _result;
	}
	
	public void SetMoveNextSource(IMoveNextSource source, UnderlyingZeroTaskToken token)
	{
		ValidateToken(token);
		ValidateContinuation();
		_moveNextSource = source;
	}

	public void SetContinuation(Action continuation, UnderlyingZeroTaskToken token)
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
	
	public UnderlyingZeroTaskToken Token { get; private set; }
	public bool ResultGot { get; private set; }

	private void ValidateToken(UnderlyingZeroTaskToken token)
	{
		if (token != Token)
		{
			throw new InvalidOperationException("Token expired.");
		}
	}

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
	private IMoveNextSource? _moveNextSource;
	private Action? _continuation;
	private TResult _result;
	private ExceptionDispatchInfo? _error;

}