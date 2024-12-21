// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct UnderlyingZeroTaskToken : IEquatable<UnderlyingZeroTaskToken>
{

	public bool Equals(UnderlyingZeroTaskToken other) => _version == other._version;
	public override bool Equals(object? obj) => obj is UnderlyingZeroTaskToken other && Equals(other);
	public override int32 GetHashCode() => _version.GetHashCode();
	public static bool operator==(UnderlyingZeroTaskToken lhs, UnderlyingZeroTaskToken rhs) => lhs.Equals(rhs);
	public static bool operator!=(UnderlyingZeroTaskToken lhs, UnderlyingZeroTaskToken rhs) => !lhs.Equals(rhs);

	public UnderlyingZeroTaskToken InlineExpired => new(INLINE_EXPIRED);

	public bool IsValid => _version > 0;
	public bool IsInlineExpired => _version == INLINE_EXPIRED;

	public UnderlyingZeroTaskToken Next
	{
		get
		{
			if (IsInlineExpired)
			{
				return this;
			}

			return new(_version + 1 != INLINE_EXPIRED ? _version + 1 : _version + 2);
		}
	}

	private const uint64 INLINE_EXPIRED = 0xDEAD;

	private UnderlyingZeroTaskToken(uint64 version) => _version = version;
	
	private readonly uint64 _version;
	
}


