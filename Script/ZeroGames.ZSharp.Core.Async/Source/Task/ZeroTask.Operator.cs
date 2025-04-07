// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public void Forget(){} // @TODO

	public ZeroTask Preserve()
		=> _backend is not null ? throw new NotImplementedException() : this;

	public ZeroTask<bool> DontThrowOnExpired()
	{
		return _backend?.GetStatus(_token) switch
		{
			null or EZeroTaskStatus.Succeeded => FromResult(false),
			EZeroTaskStatus.Canceled => FromResult(true),
			_ => throw new NotImplementedException()
		};
	}
	
	public static implicit operator ZeroTask<AsyncVoid>(ZeroTask @this)
		=> throw new NotImplementedException();

}

partial struct ZeroTask<TResult>
{
	
	public void Forget(){} // @TODO
	
	public ZeroTask<TResult> Preserve()
		=> _backend is not null ? throw new NotImplementedException() : this;
	
	public ZeroTask<(TResult? Result, bool IsCanceled)> DontThrowOnExpired()
	{
		return _backend?.GetStatus(_token) switch
		{
			null => ZeroTask.FromResult((_inlineResult: _result, false)),
			EZeroTaskStatus.Canceled => ZeroTask.FromResult((default(TResult?), true)),
			_ => throw new NotImplementedException()
		};
	}
	
	public static implicit operator ZeroTask(ZeroTask<TResult> @this)
		=> @this._backend is not null ? new(@this._backend) : ZeroTask.CompletedTask;

	public static implicit operator ZeroTask<AsyncVoid>(ZeroTask<TResult> @this)
		=> throw new NotImplementedException();

}


