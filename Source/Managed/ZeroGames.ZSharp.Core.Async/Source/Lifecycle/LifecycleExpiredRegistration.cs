// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct LifecycleExpiredRegistration : IEquatable<LifecycleExpiredRegistration>
{

	public LifecycleExpiredRegistration(ReactiveLifecycle lifecycle, uint64 handle)
	{
		Lifecycle = lifecycle;
		_handle = handle;
	}
	
	public LifecycleExpiredRegistration(ReactiveLifecycle lifecycle, CancellationTokenRegistration cancellationRegistration)
	{
		Lifecycle = lifecycle;
		_cancellationRegistration = cancellationRegistration;
	}

	public bool Equals(LifecycleExpiredRegistration other) => Lifecycle == other.Lifecycle && _handle == other._handle && _cancellationRegistration == other._cancellationRegistration;
	public override bool Equals(object? obj) => obj is LifecycleExpiredRegistration other && Equals(other);
	public override int32 GetHashCode() => _handle.GetHashCode() ^ _cancellationRegistration.GetHashCode();
	public static bool operator==(LifecycleExpiredRegistration lhs, LifecycleExpiredRegistration rhs) => lhs.Equals(rhs);
	public static bool operator!=(LifecycleExpiredRegistration lhs, LifecycleExpiredRegistration rhs) => !lhs.Equals(rhs);

	public ReactiveLifecycle Lifecycle { get; }
	private readonly uint64 _handle;
	private readonly CancellationTokenRegistration _cancellationRegistration;

}


