// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface ILifecycleBackend
{
	bool IsExpired(LifecycleToken token);
	
	LifecycleToken Token { get; }
	Lifecycle Lifecycle { get; }
}


