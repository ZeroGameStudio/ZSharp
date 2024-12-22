// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IReactiveLifecycleBackend : ILifecycleBackend
{
	LifecycleExpiredRegistration RegisterOnExpired(Action callback, LifecycleToken token);
	LifecycleExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state, LifecycleToken token);
}


