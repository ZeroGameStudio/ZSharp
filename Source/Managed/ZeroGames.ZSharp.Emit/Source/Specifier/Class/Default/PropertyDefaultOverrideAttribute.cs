// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class, AllowMultiple = true, Inherited = false)]
public class PropertyDefaultOverrideAttribute : Attribute, IUnrealReflectionSpecifier
{
	public required string Property { get; init; }
	public required object Default { get; init; }
}


