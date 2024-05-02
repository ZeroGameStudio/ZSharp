// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

[AttributeUsage(AttributeTargets.Class, Inherited = false)]
public class BuildTargetAttribute(string name) : Attribute
{
	public string Name { get; } = name;
}