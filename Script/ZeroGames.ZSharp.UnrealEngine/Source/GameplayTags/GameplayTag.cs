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

#endif


