// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IReactiveLifetimeBackend : ILifetimeBackend, IReactiveLifetimeSource
{
	LifetimeExpiredRegistration RegisterOnExpired(Action callback, LifetimeToken token);
	LifetimeExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state, LifetimeToken token);
	void UnregisterOnExpired(LifetimeExpiredRegistration registration, LifetimeToken token);
}


