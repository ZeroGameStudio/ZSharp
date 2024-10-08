// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class UPropertyAttribute : Attribute, IUnrealReflectionSpecifier
{
	public object? Default { get; init; }
}


