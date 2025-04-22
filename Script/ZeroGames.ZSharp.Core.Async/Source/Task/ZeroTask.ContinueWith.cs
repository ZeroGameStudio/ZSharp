// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public async ZeroTask ContinueWith(Action continuation)
	{
		await this;
		continuation();
	}

	public async ZeroTask ContinueWith<TState>(Action<TState> continuation, TState state)
	{
		await this;
		continuation(state);
	}

	public async ZeroTask ContinueWith(Func<ZeroTask> continuation)
	{
		await this;
		await continuation();
	}
	
	public async ZeroTask ContinueWith<TState>(Func<TState, ZeroTask> continuation, TState state)
	{
		await this;
		await continuation(state);
	}

	public async ZeroTask<T> ContinueWith<T>(Func<T> continuation)
	{
		await this;
		return continuation();
	}
	
	public async ZeroTask<T> ContinueWith<T, TState>(Func<TState, T> continuation, TState state)
	{
		await this;
		return continuation(state);
	}
	
	public async ZeroTask<T> ContinueWith<T>(Func<ZeroTask<T>> continuation)
	{
		await this;
		return await continuation();
	}
	
	public async ZeroTask<T> ContinueWith<T, TState>(Func<TState, ZeroTask<T>> continuation, TState state)
	{
		await this;
		return await continuation(state);
	}
	
}

partial struct ZeroTask<TResult>
{
	
	public async ZeroTask ContinueWith(Action continuation)
	{
		await this;
		continuation();
	}

	public async ZeroTask ContinueWith(Action<TResult> continuation)
	{
		continuation(await this);
	}
	
	public async ZeroTask ContinueWith<TState>(Action<TState> continuation, TState state)
	{
		await this;
		continuation(state);
	}

	public async ZeroTask ContinueWith<TState>(Action<TResult, TState> continuation, TState state)
	{
		continuation(await this, state);
	}
	
	public async ZeroTask ContinueWith(Func<ZeroTask> continuation)
	{
		await this;
		await continuation();
	}

	public async ZeroTask ContinueWith(Func<TResult, ZeroTask> continuation)
	{
		await continuation(await this);
	}
	
	public async ZeroTask ContinueWith<TState>(Func<TState, ZeroTask> continuation, TState state)
	{
		await this;
		await continuation(state);
	}
	
	public async ZeroTask ContinueWith<TState>(Func<TResult, TState, ZeroTask> continuation, TState state)
	{
		await continuation(await this, state);
	}
	
	public async ZeroTask<T> ContinueWith<T>(Func<T> continuation)
	{
		await this;
		return continuation();
	}

	public async ZeroTask<T> ContinueWith<T>(Func<TResult, T> continuation)
	{
		return continuation(await this);
	}
	
	public async ZeroTask<T> ContinueWith<T, TState>(Func<TState, T> continuation, TState state)
	{
		await this;
		return continuation(state);
	}
	
	public async ZeroTask<T> ContinueWith<T, TState>(Func<TResult, TState, T> continuation, TState state)
	{
		return continuation(await this, state);
	}
	
	public async ZeroTask<T> ContinueWith<T>(Func<ZeroTask<T>> continuation)
	{
		await this;
		return await continuation();
	}
	
	public async ZeroTask<T> ContinueWith<T>(Func<TResult, ZeroTask<T>> continuation)
	{
		return await continuation(await this);
	}
	
	public async ZeroTask<T> ContinueWith<T, TState>(Func<TState, ZeroTask<T>> continuation, TState state)
	{
		await this;
		return await continuation(state);
	}
	
	public async ZeroTask<T> ContinueWith<T, TState>(Func<TResult, TState, ZeroTask<T>> continuation, TState state)
	{
		return await continuation(await this, state);
	}
	
}


