// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class ConfigNameAttribute(string name) : ClassSpecifierBase
{
	public string Name { get; } = name;
}


