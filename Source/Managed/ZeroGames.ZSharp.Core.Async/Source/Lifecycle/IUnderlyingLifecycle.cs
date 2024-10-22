// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IUnderlyingLifecycle
{
	bool IsExpired(UnderlyingLifecycleToken token);
	UnderlyingLifecycleToken Token { get; }
}


