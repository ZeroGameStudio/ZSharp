// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct LifecycleExpiredRegistration : IEquatable<LifecycleExpiredRegistration>
{

	public LifecycleExpiredRegistration(ReactiveLifecycle lifecycle, uint64 handle)
	{
		_lifecycle = lifecycle;
		_handle = handle;
	}

	public bool Equals(LifecycleExpiredRegistration other) => _lifecycle == other._lifecycle && _handle == other._handle && _explicit == other._explicit;
	public override bool Equals(object? obj) => obj is LifecycleExpiredRegistration other && Equals(other);
	public override int32 GetHashCode() => _handle.GetHashCode();
	public static bool operator==(LifecycleExpiredRegistration lhs, LifecycleExpiredRegistration rhs) => lhs.Equals(rhs);
	public static bool operator!=(LifecycleExpiredRegistration lhs, LifecycleExpiredRegistration rhs) => !lhs.Equals(rhs);

	public void Unregister() => _lifecycle.UnregisterOnExpired(this);
	
	public bool IsValid => _lifecycle.IsValidRegistration(this);

	internal LifecycleExpiredRegistration(ReactiveLifecycle lifecycle, ExplicitLifecycleExpiredRegistration @explicit)
	{
		_lifecycle = lifecycle;
		_explicit = @explicit;
	}

	internal ExplicitLifecycleExpiredRegistration Explicit => _explicit;
	
	private readonly ReactiveLifecycle _lifecycle;
	private readonly uint64 _handle;
	private readonly ExplicitLifecycleExpiredRegistration _explicit;

}


