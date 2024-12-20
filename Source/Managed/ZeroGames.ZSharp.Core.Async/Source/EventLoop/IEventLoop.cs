// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public delegate void StatelessEventLoopCallback(in EventLoopArgs args, ref bool @continue);
public delegate void StatefulEventLoopCallback(in EventLoopArgs args, object? state, ref bool @continue);

public interface IEventLoop
{
	public static IEventLoop Instance => EventLoop.Instance;

	EventLoopRegistration Register(EEventLoopTickingGroup group, StatelessEventLoopCallback callback, Lifecycle lifecycle = default, Action<LifecycleExpiredException>? onExpired = null);
	EventLoopRegistration Register(EEventLoopTickingGroup group, StatefulEventLoopCallback callback, object? state, Lifecycle lifecycle = default, Action<LifecycleExpiredException>? onExpired = null);
	void Unregister(EventLoopRegistration registration);
	void UnregisterAll(Lifecycle lifecycle);
	bool IsValidRegistration(EventLoopRegistration registration);
}


