// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class BlueprintEventAttribute : Attribute, IUnrealReflectionSpecifier
{
	public string? ImplementationZCallName { get; init; }
}


