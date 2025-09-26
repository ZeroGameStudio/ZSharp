// Copyright Zero Games. All Rights Reserved.

using System.Runtime.ExceptionServices;

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_Memoize<TResult> : IZeroTaskBackend<TResult>, IMoveNextSource
{

	public ZeroTaskBackend_Memoize(IZeroTaskBackend backend)
	{
		check(typeof(TResult) == typeof(AsyncVoid));

		ZeroTaskToken token = backend.Token;
		
		_backend = backend;
		Token = token;

		if (backend.GetStatus(token) is EZeroTaskStatus.Pending)
		{
			backend.SetMoveNextSource(this, token);
		}
		else
		{
			try
			{
				_backend.GetResult(Token);
				SetResult(default!);
			}
			catch (Exception ex)
			{
				SetException(ex);
			}
		}
	}

	public ZeroTaskBackend_Memoize(IZeroTaskBackend<TResult> backend)
	{
		ZeroTaskToken token = backend.Token;
		
		_backend = backend;
		Token = token;
		
		if (backend.GetStatus(token) is EZeroTaskStatus.Pending)
		{
			backend.SetMoveNextSource(this, token);
		}
		else
		{
			try
			{
				SetResult(backend.GetResult(Token));
			}
			catch (Exception ex)
			{
				SetException(ex);
			}
		}
	}

	public EZeroTaskStatus GetStatus(ZeroTaskToken token)
	{
		if (_backend is null)
		{
			if (_error is not null)
			{
				return _error.SourceException is OperationCanceledException ? EZeroTaskStatus.Canceled : EZeroTaskStatus.Faulted;
			}
			else
			{
				return EZeroTaskStatus.Succeeded;
			}
		}

		return _backend.GetStatus(Token);
	}

	public TResult GetResult(ZeroTaskToken token)
	{
		if (_backend is not null)
		{
			throw new InvalidOperationException("ZeroTask only supports await.");
		}
		
		_error?.Throw();
		return _result;
	}

	void IZeroTaskBackend.GetResult(ZeroTaskToken token)
		=> GetResult(token);

	public void SetContinuation(Action continuation, ZeroTaskToken token)
	{
		check(_backend is not null);
		_continuations.Add(new(continuation, null));
	}

	public void SetMoveNextSource(IMoveNextSource source, ZeroTaskToken token)
	{
		check(_backend is not null);
		_continuations.Add(new(null, source));
	}

	void IMoveNextSource.MoveNext()
	{
		try
		{
			if (_backend is IZeroTaskBackend<TResult> typedBackend)
			{
				SetResult(typedBackend.GetResult(Token));
			}
			else
			{
				check(typeof(TResult) == typeof(AsyncVoid));
					
				_backend!.GetResult(Token);
				SetResult(default!);
			}
		}
		catch (Exception ex)
		{
			SetException(ex);
		}
	}
	
	public ZeroTaskToken Token { get; }
	
	private readonly record struct ContinuationVariant(Action? Action, IMoveNextSource? Source);

	private void SetResult(TResult result)
	{
		_result = result;
		SignalCompletion();
	}

	private void SetException(Exception exception)
	{
		_error = ExceptionDispatchInfo.Capture(exception);
		SignalCompletion();
	}
	
	private void SignalCompletion()
	{
		_backend = null;

		foreach (var continuation in _continuations)
		{
			try
			{
				if (continuation.Action is not null)
				{
					continuation.Action();
				}
				else
				{
					continuation.Source!.MoveNext();
				}
			}
			catch (Exception ex)
			{
				UnobservedZeroTaskExceptionPublisher.Publish(ex);
			}
		}
	}

	private IZeroTaskBackend? _backend;
	private TResult _result = default!;
	private ExceptionDispatchInfo? _error;
	private readonly List<ContinuationVariant> _continuations = [];
}


