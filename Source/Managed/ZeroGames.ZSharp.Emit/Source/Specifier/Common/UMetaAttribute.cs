// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class UMetaAttribute(string firstPair, params string[]? pairs) : CommonSpecifierBase
{
	public string[] Pairs { get; } = pairs is not null ? pairs.Prepend(firstPair).ToArray() : [ firstPair ];
}


