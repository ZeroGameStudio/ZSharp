// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Method)]
public sealed class ZCallAttribute : Attribute, IZCallAttribute
{
	public string? Name { get; init; }
}


