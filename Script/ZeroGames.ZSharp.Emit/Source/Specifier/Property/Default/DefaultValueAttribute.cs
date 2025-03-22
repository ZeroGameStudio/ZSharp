// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class DefaultValueAttribute(object? value) : PropertySpecifierBase
{
	public object? Value { get; } = value;
}


