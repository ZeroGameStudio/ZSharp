// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class TooltipAttribute(string value) : CommonSpecifierBase
{
	public string Value { get; } = value;
}


