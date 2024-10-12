// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class AutoExpandCategoriesAttribute(string firstName, params string[]? names) : ClassSpecifierBase
{
	public string[] Names { get; } = names is not null ? names.Prepend(firstName).ToArray() : [ firstName ];
}


