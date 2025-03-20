// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class CategoryAttribute(string value) : CommonSpecifierBase
{
	public string Value { get; } = value;
}


