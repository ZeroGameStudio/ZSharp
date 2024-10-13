// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class FieldNotifyAttribute : PropertySpecifierBase
{
	public IEnumerable<string>? RelatedFields { get; init; }
}


