// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

[AttributeUsage(AttributeTargets.Parameter)]
public class ArgumentAttribute(string name) : Attribute
{
	public string Name { get; } = name;
}


