// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public class ZCallAttribute : Attribute
{
	public ZCallAttribute(){}
	public ZCallAttribute(string name) => Name = name;
	public string? Name { get; }
}


