// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public void Forget(){} // @TODO

	public ZeroTask Preserve()
		=> _backend is not { IsPreserved: false } ? this : throw new NotImplementedException();

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

	public static implicit operator ZeroTask<AsyncVoid>(ZeroTask @this)
		=> @this.ToAsyncVoidTask();

	private async ZeroTask<AsyncVoid> ToAsyncVoidTask()
	{
		await this;
		return default;
	}

}

partial struct ZeroTask<TResult>
{
	
	public void Forget(){} // @TODO
	
	public ZeroTask<TResult> Preserve()
		=> _backend is not { IsPreserved: false } ? this : throw new NotImplementedException();
	
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
	
	public static implicit operator ZeroTask(ZeroTask<TResult> @this)
		=> @this._backend is not null ? new(@this._backend) : ZeroTask.CompletedTask;

	public static implicit operator ZeroTask<AsyncVoid>(ZeroTask<TResult> @this)
		=> @this.ToAsyncVoidTask();
	
	private async ZeroTask<AsyncVoid> ToAsyncVoidTask()
	{
		await this;
		return default;
	}

}


