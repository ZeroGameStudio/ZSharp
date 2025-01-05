// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.Core.Async;

public interface ITimerScheduler
{
	void Unregister(Timer timer);
	void Suspend(Timer timer);
	void Resume(Timer timer);
	bool IsValidTimer(Timer timer);
}

public interface ITimerScheduler<T> : ITimerScheduler where T : struct, INumber<T>
{
	Timer Register(Action<T> callback, T rate, bool looped = false, bool fixedRate = true, Lifecycle lifecycle = default, Action<LifecycleExpiredException>? onExpired = null);
	Timer Register(Action<T, object?> callback, object? state, T rate, bool looped = false, bool fixedRate = true, Lifecycle lifecycle = default, Action<LifecycleExpiredException, object?>? onExpired = null);
}


