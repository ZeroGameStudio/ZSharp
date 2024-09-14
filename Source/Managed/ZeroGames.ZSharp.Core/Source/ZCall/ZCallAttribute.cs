// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Method)]
public class ZCallAttribute() : Attribute
{
	public ZCallAttribute(string name) : this() => Name = name;
	public string? Name { get; }
}


