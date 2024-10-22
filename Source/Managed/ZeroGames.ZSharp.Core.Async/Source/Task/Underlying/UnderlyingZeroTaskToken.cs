// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct UnderlyingZeroTaskToken(uint64 version) : IEquatable<UnderlyingZeroTaskToken>
{

	public bool Equals(UnderlyingZeroTaskToken other) => _version == other._version;
	public override bool Equals(object? obj) => obj is UnderlyingZeroTaskToken other && Equals(other);
	public override int32 GetHashCode() => _version.GetHashCode();
	public static bool operator==(UnderlyingZeroTaskToken lhs, UnderlyingZeroTaskToken rhs) => lhs.Equals(rhs);
	public static bool operator!=(UnderlyingZeroTaskToken lhs, UnderlyingZeroTaskToken rhs) => !lhs.Equals(rhs);

	public bool IsValid => _version > 0;
	
	public UnderlyingZeroTaskToken Next => new(_version + 1);
	
	private readonly uint64 _version = version;
	
}


