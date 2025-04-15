// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class GetOptionsAttribute(string function) : PropertySpecifierBase
{
	public string Function { get; } = function;
}


