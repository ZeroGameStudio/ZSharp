﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IReactiveLifecycleBackend : ILifecycleBackend, IReactiveLifecycleSource
{
	LifecycleExpiredRegistration RegisterOnExpired(Action callback, LifecycleToken token);
	LifecycleExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state, LifecycleToken token);
	void UnregisterOnExpired(LifecycleExpiredRegistration registration, LifecycleToken token);
}


