// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class EditorConfigNameAttribute(string name) : ClassSpecifierBase
{
	public string Name { get; } = name;
}


