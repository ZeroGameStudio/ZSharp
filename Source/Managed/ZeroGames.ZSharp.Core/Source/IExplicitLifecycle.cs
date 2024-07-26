// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IExplicitLifecycle
{
	bool IsExpired { get; }
	ExplicitLifecycleExpiredRegistration RegisterOnExpired(Action<IExplicitLifecycle, object?> callback, object? state);
	void UnregisterOnExpired(ExplicitLifecycleExpiredRegistration registration);
}


