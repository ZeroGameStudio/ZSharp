// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public interface IReactiveUnderlyingLifecycle : IUnderlyingLifecycle
{
	LifecycleExpiredRegistration RegisterOnExpired(Action<IReactiveUnderlyingLifecycle, object?> callback, object? state, UnderlyingLifecycleToken token);
	void UnregisterOnExpired(LifecycleExpiredRegistration registration, UnderlyingLifecycleToken token);
	bool IsValidRegistration(LifecycleExpiredRegistration registration);
}


