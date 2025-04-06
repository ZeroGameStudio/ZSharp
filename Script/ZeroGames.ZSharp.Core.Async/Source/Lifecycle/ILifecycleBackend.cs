// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface ILifecycleBackend : ILifecycleSource
{
	bool IsExpired(LifecycleToken token);
	
	LifecycleToken Token { get; }
}


