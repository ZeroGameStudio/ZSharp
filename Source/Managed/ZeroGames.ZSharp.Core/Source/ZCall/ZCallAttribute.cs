// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Method)]
public class ZCallAttribute : Attribute
{
	public string? Name { get; init; }
}


