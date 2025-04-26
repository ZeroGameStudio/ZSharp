// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	// Forget must return void so we expand the state machine manually.
	public void Forget(Action<Exception>? exceptionHandler = null)
	{
		Awaiter awaiter = GetAwaiter();
		if (awaiter.IsCompleted)
		{
			try
			{
				awaiter.GetResult();
			}
			catch (Exception ex)
			{
				(exceptionHandler ?? _defaultExceptionHandler)(ex);
			}
		}
		else
		{
			var backend = ZeroTaskBackend_AsyncStateMachine<AsyncVoid, ForgotStateMachine>.GetFromPool();
			backend.StateMachine = new(awaiter, exceptionHandler ?? _defaultExceptionHandler);
			((IMoveNextSourceAwaiter)awaiter).OnCompleted(backend);
		}
	}

	public ZeroTask Preserve()
		=> _backend is IReusedZeroTaskBackend ? new(new ZeroTaskBackend_Memoize<AsyncVoid>(_backend)) : this;

	public async ZeroTask<bool> DontThrowOnExpired()
	{
		try
		{
			await this;
			return false;
		}
		catch (OperationCanceledException)
		{
			return true;
		}
	}
	
	public async ZeroTask<T> WithResult<T>(T result)
	{
		await this;
		return result;
	}
	
	public async ZeroTask<AsyncVoid> ToAsyncVoidTask()
	{
		await this;
		return default;
	}

	public static implicit operator ZeroTask<AsyncVoid>(ZeroTask @this)
		=> @this.ToAsyncVoidTask();

	private readonly struct ForgotStateMachine(Awaiter awaiter, Action<Exception> exceptionHandler) : IAsyncStateMachine
	{
		void IAsyncStateMachine.MoveNext()
		{
			try
			{
				_awaiter.GetResult();
			}
			catch (Exception ex)
			{
				_exceptionHandler(ex);
			}
		}

		void IAsyncStateMachine.SetStateMachine(IAsyncStateMachine stateMachine){}
		
		private readonly Awaiter _awaiter = awaiter;
		private readonly Action<Exception> _exceptionHandler = exceptionHandler;
	}
	
	private static readonly Action<Exception> _defaultExceptionHandler = UnobservedZeroTaskExceptionPublisher.Publish;

}

partial struct ZeroTask<TResult>
{

	// We don't care about the result so forward to ZeroTask directly.
	public void Forget(Action<Exception>? exceptionHandler = null) => ((ZeroTask)this).Forget(exceptionHandler);
	
	public ZeroTask<TResult> Preserve()
		=> _backend is IReusedZeroTaskBackend ? new(new ZeroTaskBackend_Memoize<TResult>(_backend)) : this;
	
	public async ZeroTask<(TResult? Result, bool IsCanceled)> DontThrowOnExpired()
	{
		try
		{
			return (await this, false);
		}
		catch (OperationCanceledException)
		{
			return (default, true);
		}
	}
	
	public async ZeroTask<T> WithResult<T>(T result)
	{
		await this;
		return result;
	}

	public ZeroTask ToZeroTask()
		=> _backend is not null ? new(_backend) : ZeroTask.CompletedTask;
	
	public async ZeroTask<AsyncVoid> ToAsyncVoidTask()
	{
		await this;
		return default;
	}
	
	public static implicit operator ZeroTask(ZeroTask<TResult> @this)
		=> @this.ToZeroTask();

	public static implicit operator ZeroTask<AsyncVoid>(ZeroTask<TResult> @this)
		=> @this.ToAsyncVoidTask();

}


