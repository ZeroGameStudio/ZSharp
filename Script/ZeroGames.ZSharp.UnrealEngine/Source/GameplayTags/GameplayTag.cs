// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_GAMEPLAYTAGS

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.GameplayTags;

public partial class GameplayTag
{

	public static GameplayTag Request(string tagName)
	{
		if (!TryRequest(tagName, out var tag))
		{
			throw new ArgumentOutOfRangeException(nameof(tagName));
		}

		return tag;
	}
	
	public static bool TryRequest(string tagName, [NotNullWhen(true)] out GameplayTag? tag)
	{
		MasterAlcCache.GuardInvariant();
		tag = InternalRequest(tagName);
		return tag is not null;
	}

	private static unsafe GameplayTag? InternalRequest(string tagName)
	{
		fixed (char* buffer = tagName)
		{
			return GameplayTag_Interop.Request(buffer).GetTarget<GameplayTag>();
		}
	}

}

#endif


