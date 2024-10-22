// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public struct AsyncZeroTaskMethodBuilderVoid : IAsyncMethodBuilder<AsyncZeroTaskMethodBuilderVoid, ZeroTask, ZeroTask.Awaiter>
{

	public static AsyncZeroTaskMethodBuilderVoid Create() => default;

	public void SetStateMachine(IAsyncStateMachine stateMachine) => throw new NotSupportedException();
	
	public void Start<TStateMachine>(ref TStateMachine stateMachine) where TStateMachine : IAsyncStateMachine => stateMachine.MoveNext();

	public void AwaitOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : INotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		_task ??= ZeroTask_AsyncStateMachine<AsyncVoid>.GetFromPool();
		
		AsyncZeroTaskMethodBuilderShared.AwaitOnCompleted(ref awaiter, ref stateMachine);
	}

	public void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		_task ??= ZeroTask_AsyncStateMachine<AsyncVoid>.GetFromPool();
		
		AsyncZeroTaskMethodBuilderShared.AwaitUnsafeOnCompleted(ref awaiter, ref stateMachine);
	}

	public void SetResult()
	{
		if (_task is not null)
		{
			_task.SetResult(default);
		}
	}

	public void SetException(Exception exception)
	{
		if (_task is not null)
		{
			_task.SetException(exception);
		}
		else
		{
			_exception = exception;
		}
	}

	public ZeroTask Task
	{
		get
		{
			if (_task is not null)
			{
				return _task.Task;
			}
			else if (_exception is not null)
			{
				return ZeroTask.FromException(_exception);
			}
			else
			{
				return ZeroTask.CompletedTask;
			}
		}
	}

	private ZeroTask_AsyncStateMachine<AsyncVoid>? _task;
	private Exception? _exception;

}

public struct AsyncZeroTaskMethodBuilder<TResult> : IAsyncMethodBuilder<TResult, AsyncZeroTaskMethodBuilder<TResult>, ZeroTask<TResult>, ZeroTask<TResult>.Awaiter>
{

	public static AsyncZeroTaskMethodBuilder<TResult> Create() => default;

	public void SetStateMachine(IAsyncStateMachine stateMachine) => throw new NotSupportedException();
	
	public void Start<TStateMachine>(ref TStateMachine stateMachine) where TStateMachine : IAsyncStateMachine => stateMachine.MoveNext();

	public void AwaitOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : INotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		_task ??= ZeroTask_AsyncStateMachine<TResult>.GetFromPool();
		
		AsyncZeroTaskMethodBuilderShared.AwaitOnCompleted(ref awaiter, ref stateMachine);
	}

	public void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		_task ??= ZeroTask_AsyncStateMachine<TResult>.GetFromPool();
		
		AsyncZeroTaskMethodBuilderShared.AwaitUnsafeOnCompleted(ref awaiter, ref stateMachine);
	}

	void IAsyncMethodBuilder<AsyncZeroTaskMethodBuilder<TResult>, ZeroTask<TResult>, ZeroTask<TResult>.Awaiter>.SetResult() => throw new NotSupportedException();
	public void SetResult(TResult result)
	{
		if (_task is not null)
		{
			_task.SetResult(result);
		}
		else
		{
			_inlineResult = result;
		}
	}

	public void SetException(Exception exception)
	{
		if (_task is not null)
		{
			_task.SetException(exception);
		}
		else
		{
			_exception = exception;
		}
	}

	public ZeroTask<TResult> Task
	{
		get
		{
			if (_task is not null)
			{
				return _task.Task;
			}
			else if (_exception is not null)
			{
				return ZeroTask.FromException<TResult>(_exception);
			}
			else
			{
				return ZeroTask.FromResult<TResult>(_inlineResult!);
			}
		}
	}

	private TResult? _inlineResult;
	private ZeroTask_AsyncStateMachine<TResult>? _task;
	private Exception? _exception;

}

internal static class AsyncZeroTaskMethodBuilderShared
{
	
	public static void AwaitOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : INotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		ThreadHelper.ValidateGameThread();
		
		// IZeroTaskAwaiter is internal and only implemented by struct ZeroTask.Awaiter.
		// The null tests here ensure that the jit can optimize away the interface tests when TAwaiter is a ref type.
		if (default(TAwaiter) is not null && awaiter is IZeroTaskAwaiter zeroAwaiter)
		{
			zeroAwaiter.OnCompleted(stateMachine);
		}
		else
		{
			// The awaiter isn't specially known. Fall back to doing a normal await.
			TStateMachine copy = stateMachine;
			awaiter.OnCompleted(() => { copy.MoveNext(); });
		}
	}
	
	public static void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		ThreadHelper.ValidateGameThread();
		
		TStateMachine copy = stateMachine;
		awaiter.UnsafeOnCompleted(() => { copy.MoveNext(); });
	}
	
}


