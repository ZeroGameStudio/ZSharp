// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct ExplicitLifecycleExpiredRegistration(IExplicitLifecycle lifecycle, uint64 handle) : IEquatable<ExplicitLifecycleExpiredRegistration>
{
	
	public bool Equals(ExplicitLifecycleExpiredRegistration other) => _lifecycle == other._lifecycle && _handle == other._handle;
	public override bool Equals(object? obj) => obj is ExplicitLifecycleExpiredRegistration other && Equals(other);
	public override int32 GetHashCode() => _handle.GetHashCode();
	public static bool operator==(ExplicitLifecycleExpiredRegistration lhs, ExplicitLifecycleExpiredRegistration rhs) => lhs.Equals(rhs);
	public static bool operator!=(ExplicitLifecycleExpiredRegistration lhs, ExplicitLifecycleExpiredRegistration rhs) => !lhs.Equals(rhs);

	public void Unregister() => _lifecycle.UnregisterOnExpired(this);

	private readonly IExplicitLifecycle _lifecycle = lifecycle;
	private readonly uint64 _handle = handle;
	
}


