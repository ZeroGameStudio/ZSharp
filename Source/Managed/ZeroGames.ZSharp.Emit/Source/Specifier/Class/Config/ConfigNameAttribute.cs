// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class)]
public class ConfigNameAttribute(string name) : Attribute, IUnrealReflectionSpecifier
{
	public string Name { get; } = name;
}


