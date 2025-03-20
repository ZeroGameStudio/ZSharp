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
		if (_backend is null)
		{
			var backend = ZeroTaskBackend_AsyncStateMachine<AsyncVoid, TStateMachine>.GetFromPool();
			backend.StateMachine = stateMachine;
			_backend = backend;
			Task = ZeroTask.FromBackend(backend);
		}
		
		AsyncZeroTaskMethodBuilderShared.AwaitOnCompleted(ref awaiter, _backend);
	}

	public void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine
		=> AwaitOnCompleted(ref awaiter, ref stateMachine);

	public void SetResult()
	{
		if (_backend is not null)
		{
			_backend.SetResult(default);
		}
		else
		{
			Task = ZeroTask.CompletedTask;
		}
	}

	public void SetException(Exception exception)
	{
		if (_backend is not null)
		{
			_backend.SetException(exception);
		}
		else
		{
			Task = ZeroTask.FromException(exception);
		}
	}

	public ZeroTask Task { get; private set; }

	private IAsyncStateMachineTask<AsyncVoid>? _backend;

}

public struct AsyncZeroTaskMethodBuilder<TResult> : IAsyncMethodBuilder<TResult, AsyncZeroTaskMethodBuilder<TResult>, ZeroTask<TResult>, ZeroTask<TResult>.Awaiter>
{

	public static AsyncZeroTaskMethodBuilder<TResult> Create() => default;

	public void SetStateMachine(IAsyncStateMachine stateMachine) => throw new NotSupportedException();
	
	public void Start<TStateMachine>(ref TStateMachine stateMachine) where TStateMachine : IAsyncStateMachine => stateMachine.MoveNext();

	public void AwaitOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : INotifyCompletion where TStateMachine : IAsyncStateMachine
	{
		if (_backend is null)
		{
			var backend = ZeroTaskBackend_AsyncStateMachine<TResult, TStateMachine>.GetFromPool();
			backend.StateMachine = stateMachine;
			_backend = backend;
			Task = ZeroTask.FromBackend(backend);
		}
		
		AsyncZeroTaskMethodBuilderShared.AwaitOnCompleted(ref awaiter, _backend);
	}

	public void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine
		=> AwaitOnCompleted(ref awaiter, ref stateMachine);

	void IAsyncMethodBuilder<AsyncZeroTaskMethodBuilder<TResult>, ZeroTask<TResult>, ZeroTask<TResult>.Awaiter>.SetResult() => throw new NotSupportedException();
	public void SetResult(TResult result)
	{
		if (_backend is not null)
		{
			_backend.SetResult(result);
		}
		else
		{
			Task = ZeroTask.FromResult(result);
		}
	}

	public void SetException(Exception exception)
	{
		if (_backend is not null)
		{
			_backend.SetException(exception);
		}
		else
		{
			Task = ZeroTask.FromException<TResult>(exception);
		}
	}

	public ZeroTask<TResult> Task { get; private set; }

	private IAsyncStateMachineTask<TResult>? _backend;

}

internal static class AsyncZeroTaskMethodBuilderShared
{
	
	public static void AwaitOnCompleted<TAwaiter, TResult>(ref TAwaiter awaiter, IAsyncStateMachineTask<TResult> backend) where TAwaiter : INotifyCompletion
	{
		Thrower.ThrowIfNotInGameThread();
		
		// IZeroTaskAwaiter is internal and only implemented by struct ZeroTask.Awaiter.
		// The null tests here ensure that the jit can optimize away the interface tests when TAwaiter is a ref type.
		if (default(TAwaiter) is not null && awaiter is IMoveNextSourceAwaiter zeroTaskAwaiter)
		{
			zeroTaskAwaiter.OnCompleted(backend);
		}
		else
		{
			// The awaiter isn't specially known. Fall back to doing a normal await.
			awaiter.OnCompleted(backend.MoveNextDelegate);
		}
	}
	
}


