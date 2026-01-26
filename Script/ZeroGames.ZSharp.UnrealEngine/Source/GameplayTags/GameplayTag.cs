// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_GAMEPLAYTAGS

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.GameplayTags;

public partial class FGameplayTag
{

	public static FGameplayTag Request(string tagName)
	{
		if (!TryRequest(tagName, out var tag))
		{
			throw new ArgumentOutOfRangeException(nameof(tagName));
		}

		return tag;
	}
	
	public static bool TryRequest(string tagName, [NotNullWhen(true)] out FGameplayTag? tag)
	{
		MasterAlcCache.GuardInvariant();
		tag = InternalRequest(tagName);
		return tag is not null;
	}

	private static unsafe FGameplayTag? InternalRequest(string tagName)
	{
		fixed (char* buffer = tagName)
		{
			return GameplayTag_Interop.Request(buffer).GetTarget<FGameplayTag>();
		}
	}

}

public readonly partial struct GameplayTag : IEquatable<GameplayTag>
{

    public static GameplayTag Request(string tagName)
    {
        _ = FGameplayTag.Request(tagName);
        return new(tagName);
    }

    public static GameplayTag EmptyTag => new();
    
    public static IEqualityComparer<GameplayTag> DefaultEqualityComparer { get; } = new EqualityComparer();

    public static bool TryRequest(string tagName, out GameplayTag tag)
    {
        if (!FGameplayTag.TryRequest(tagName, out _))
        {
            tag = default;
            return false;
        }
        
        tag = new(tagName);
        return true;
    }
    
    public static implicit operator GameplayTag(FGameplayTag tag) => new(UBlueprintGameplayTagLibrary.GetTagName(tag));
    public static implicit operator FGameplayTag(GameplayTag tag) => FGameplayTag.Request(tag.TagName);
    
    public static bool operator ==(GameplayTag lhs, GameplayTag rhs) => lhs.Equals(rhs);
    public static bool operator !=(GameplayTag lhs, GameplayTag rhs) => !lhs.Equals(rhs);
    
    public GameplayTag()
    {
        TagName = Name_None;
    }
    
    public bool MatchesTag(GameplayTag tagToCheck)
    {
	    return UBlueprintGameplayTagLibrary.MatchesTag(this, tagToCheck, false);
    }

    public bool MatchesTagExact(GameplayTag tagToCheck)
    {
	    return tagToCheck.IsValid && this == tagToCheck;
    }

    public int32 MatchesTagDepth(GameplayTag tagToCheck)
    {
	    throw new NotImplementedException();
    }

    public bool MatchesAny(ReadOnlyGameplayTagContainer containerToCheck)
    {
	    return UBlueprintGameplayTagLibrary.MatchesAnyTags(this, containerToCheck, false);
    }
    
    public bool MatchesAnyExact(ReadOnlyGameplayTagContainer containerToCheck)
    {
	    return containerToCheck.Tags.Contains(this);
    }

    public GameplayTagContainer ToSingleTagContainer()
    {
	    GameplayTagContainer container = new();
	    container.AddTag(this);
	    return container;
    }

    public string TagName => field ?? Name_None;
    
    public bool IsValid => TagName != Name_None;

    private sealed class EqualityComparer : IEqualityComparer<GameplayTag>
    {
        public bool Equals(GameplayTag lhs, GameplayTag rhs) => lhs == rhs;
        public int32 GetHashCode(GameplayTag other) => other.GetHashCode();
    }
    
    private GameplayTag(string tagName)
    {
        TagName = tagName;
    }

    private static string Name_None => field ??= new FName().Data;
    
    #region IEquatable<GameplayTag> Implementations
    
    public bool Equals(GameplayTag other) => TagName.Equals(other.TagName, StringComparison.OrdinalIgnoreCase);
    
    #endregion
    
    #region Overrides
    
    public override bool Equals(object? obj) => obj is GameplayTag other && Equals(other);
    public override int32 GetHashCode() => TagName.GetHashCode();
    public override string ToString() => TagName;

    #endregion

}

#endif


