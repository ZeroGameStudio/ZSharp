// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface ILifetimeBackend : ILifetimeSource
{
	bool IsExpired(LifetimeToken token);
	
	LifetimeToken Token { get; }
}


