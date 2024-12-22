// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal interface IPoolableZeroTaskBackend<out TResult, TImpl> : IZeroTaskBackend<TResult> where TImpl : class, IPoolableZeroTaskBackend<TResult, TImpl>
{
	void Initialize();
	void Deinitialize();
	protected internal TImpl? PoolNext { get; set; }
}


