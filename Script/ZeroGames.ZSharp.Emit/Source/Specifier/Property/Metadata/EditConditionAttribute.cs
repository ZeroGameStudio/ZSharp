// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class EditConditionAttribute(string condition) : PropertySpecifierBase
{
	public string Condition { get; } = condition;
	public bool Hides { get; init; }
}


