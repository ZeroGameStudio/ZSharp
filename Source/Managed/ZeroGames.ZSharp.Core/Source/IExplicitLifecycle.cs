// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IExplicitLifecycle
{
	ExplicitLifecycleExpiredRegistration RegisterOnExpired(Action<IExplicitLifecycle, object?> callback, object? state);
	void UnregisterOnExpired(ExplicitLifecycleExpiredRegistration registration);
	bool IsValidRegistration(ExplicitLifecycleExpiredRegistration registration);
	
	bool IsExpired { get; }
	
	bool IsGameThreadOnly { get; }
	object SyncRoot { get; }
}


