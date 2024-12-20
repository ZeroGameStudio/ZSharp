// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IReactiveUnderlyingLifecycle : IUnderlyingLifecycle
{
	LifecycleExpiredRegistration RegisterOnExpired(Action callback, UnderlyingLifecycleToken token);
	LifecycleExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state, UnderlyingLifecycleToken token);
}


