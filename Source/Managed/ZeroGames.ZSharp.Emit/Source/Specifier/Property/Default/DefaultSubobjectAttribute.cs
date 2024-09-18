// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class DefaultSubobjectAttribute : Attribute
{
	public string? Name { get; init; }
}


