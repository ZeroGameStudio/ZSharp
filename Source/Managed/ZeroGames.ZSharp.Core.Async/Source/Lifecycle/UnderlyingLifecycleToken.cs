// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct UnderlyingLifecycleToken(uint64 version) : IEquatable<UnderlyingLifecycleToken>
{

	public bool Equals(UnderlyingLifecycleToken other) => _version == other._version;
	public override bool Equals(object? obj) => obj is UnderlyingLifecycleToken other && Equals(other);
	public override int32 GetHashCode() => _version.GetHashCode();
	public static bool operator==(UnderlyingLifecycleToken lhs, UnderlyingLifecycleToken rhs) => lhs.Equals(rhs);
	public static bool operator!=(UnderlyingLifecycleToken lhs, UnderlyingLifecycleToken rhs) => !lhs.Equals(rhs);

	public bool IsValid => _version > 0;

	public UnderlyingLifecycleToken Next => new(_version + 1);
	
	private readonly uint64 _version = version;
	
}


