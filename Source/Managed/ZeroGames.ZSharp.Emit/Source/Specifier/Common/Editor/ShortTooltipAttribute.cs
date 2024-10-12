// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class ShortTooltipAttribute(string value) : CommonSpecifierBase
{
	public string Value { get; } = value;
}


