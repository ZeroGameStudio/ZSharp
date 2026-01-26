// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_GAMEPLAYTAGS

namespace ZeroGames.ZSharp.UnrealEngine.GameplayTags;

public readonly struct GameplayTagContainerToken : IEquatable<GameplayTagContainerToken>
{
	
	public bool Equals(GameplayTagContainerToken other) => _version == other._version;
	public override bool Equals(object? obj) => obj is GameplayTagContainerToken other && Equals(other);
	public override int32 GetHashCode() => _version.GetHashCode();
	public static bool operator==(GameplayTagContainerToken lhs, GameplayTagContainerToken rhs) => lhs.Equals(rhs);
	public static bool operator!=(GameplayTagContainerToken lhs, GameplayTagContainerToken rhs) => !lhs.Equals(rhs);
	
	public static GameplayTagContainerToken First => new(1);
	
	public bool IsValid => _version > 0;

	public GameplayTagContainerToken Next => new(_version + 1);
	
	private GameplayTagContainerToken(uint64 version) => _version = version;
	
	private readonly uint64 _version;
	
}

#endif


