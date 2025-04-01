// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class DefaultToSelfAttribute(string name) : FunctionSpecifierBase
{
	public string Name { get; } = name;
}


