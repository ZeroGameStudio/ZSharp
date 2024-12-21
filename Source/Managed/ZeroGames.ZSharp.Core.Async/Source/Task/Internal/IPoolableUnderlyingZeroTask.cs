// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal interface IPoolableUnderlyingZeroTask<out TResult, TImpl> : IUnderlyingZeroTask<TResult> where TImpl : class, IPoolableUnderlyingZeroTask<TResult, TImpl>
{
	void Initialize();
	void Deinitialize();
	protected internal TImpl? PoolNext { get; set; }
}


