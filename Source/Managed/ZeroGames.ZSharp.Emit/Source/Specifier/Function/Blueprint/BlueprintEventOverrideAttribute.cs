// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class BlueprintEventOverrideAttribute : Attribute, IUnrealReflectionSpecifier
{
	public required string EventName { get; init; }
}


