// Copyright Zero Games. All Rights Reserved.

namespace System.Runtime.CompilerServices;

public interface IAsyncMethodBuilder<out TImpl, out TTask, TTaskAwaiter> where TImpl : struct, IAsyncMethodBuilder<TImpl, TTask, TTaskAwaiter> where TTask : IAwaitable<TTaskAwaiter> where TTaskAwaiter : struct, IAwaiter
{
	static abstract TImpl Create();
	void SetStateMachine(IAsyncStateMachine stateMachine);
	void Start<TStateMachine>(ref TStateMachine stateMachine) where TStateMachine : IAsyncStateMachine;
	void AwaitOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : INotifyCompletion where TStateMachine : IAsyncStateMachine;
	void AwaitUnsafeOnCompleted<TAwaiter, TStateMachine>(ref TAwaiter awaiter, ref TStateMachine stateMachine) where TAwaiter : ICriticalNotifyCompletion where TStateMachine : IAsyncStateMachine;
	void SetResult();
	void SetException(Exception exception);
	TTask Task { get; }
}

public interface IAsyncMethodBuilder<in TResult, out TImpl, out TTask, TTaskAwaiter> : IAsyncMethodBuilder<TImpl, TTask, TTaskAwaiter> where TImpl : struct, IAsyncMethodBuilder<TResult, TImpl, TTask, TTaskAwaiter> where TTask : IAwaitable<TResult, TTaskAwaiter> where TTaskAwaiter : struct, IAwaiter<TResult>
{
	void SetResult(TResult result);
}


