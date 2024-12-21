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
		if (_underlyingTask is null)
		{
			var underlyingTask = ZeroTask_AsyncStateMachine<AsyncVoid, TStateMachine>.GetFromPool();
			underlyingTask.StateMachine = stateMachine;
			_underlyingTask = underlyingTask;
			Task = ZeroTask.FromUnderlyingTask(underlyingTask);
		}
		
		AsyncZeroTaskMethodBuilderShared.AwaitOnCompleted(ref awaiter, _underlyingTask);
	}

	public void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine
		=> AwaitOnCompleted(ref awaiter, ref stateMachine);

	public void SetResult()
	{
		if (_underlyingTask is not null)
		{
			_underlyingTask.SetResult(default);
		}
		else
		{
			Task = ZeroTask.CompletedTask;
		}
	}

	public void SetException(Exception exception)
	{
		if (_underlyingTask is not null)
		{
			_underlyingTask.SetException(exception);
		}
		else
		{
			Task = ZeroTask.FromException(exception);
		}
	}

	public ZeroTask Task { get; private set; }

	private IAsyncStateMachineTask<AsyncVoid>? _underlyingTask;

}

public struct AsyncZeroTaskMethodBuilder<TResult> : IAsyncMethodBuilder<TResult, AsyncZeroTaskMethodBuilder<TResult>, ZeroTask<TResult>, ZeroTask<TResult>.Awaiter>
{

	public static AsyncZeroTaskMethodBuilder<TResult> Create() => default;

	public void SetStateMachine(IAsyncStateMachine stateMachine) => throw new NotSupportedException();
	
	public void Start<TStateMachine>(ref TStateMachine stateMachine) where TStateMachine : IAsyncStateMachine => stateMachine.MoveNext();

	public void AwaitOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : INotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		if (_underlyingTask is null)
		{
			var underlyingTask = ZeroTask_AsyncStateMachine<TResult, TStateMachine>.GetFromPool();
			underlyingTask.StateMachine = stateMachine;
			_underlyingTask = underlyingTask;
			Task = ZeroTask.FromUnderlyingTask(underlyingTask);
		}
		
		AsyncZeroTaskMethodBuilderShared.AwaitOnCompleted(ref awaiter, _underlyingTask);
	}

	public void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine
		=> AwaitOnCompleted(ref awaiter, ref stateMachine);

	void IAsyncMethodBuilder<AsyncZeroTaskMethodBuilder<TResult>, ZeroTask<TResult>, ZeroTask<TResult>.Awaiter>.SetResult() => throw new NotSupportedException();
	public void SetResult(TResult result)
	{
		if (_underlyingTask is not null)
		{
			_underlyingTask.SetResult(result);
		}
		else
		{
			Task = ZeroTask.FromResult(result);
		}
	}

	public void SetException(Exception exception)
	{
		if (_underlyingTask is not null)
		{
			_underlyingTask.SetException(exception);
		}
		else
		{
			Task = ZeroTask.FromException<TResult>(exception);
		}
	}

	public ZeroTask<TResult> Task { get; private set; }

	private IAsyncStateMachineTask<TResult>? _underlyingTask;

}

internal static class AsyncZeroTaskMethodBuilderShared
{
	
	public static void AwaitOnCompleted<TAwaiter, TResult>(ref TAwaiter awaiter, IAsyncStateMachineTask<TResult> underlyingTask) where TAwaiter : INotifyCompletion
	{
		Thrower.ThrowIfNotInGameThread();
		
		// IZeroTaskAwaiter is internal and only implemented by struct ZeroTask.Awaiter.
		// The null tests here ensure that the jit can optimize away the interface tests when TAwaiter is a ref type.
		if (default(TAwaiter) is not null && awaiter is IMoveNextSourceAwaiter zeroTaskAwaiter)
		{
			zeroTaskAwaiter.OnCompleted(underlyingTask);
		}
		else
		{
			// The awaiter isn't specially known. Fall back to doing a normal await.
			awaiter.OnCompleted(underlyingTask.MoveNextDelegate);
		}
	}
	
}


