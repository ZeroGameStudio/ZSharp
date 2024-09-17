// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class BlueprintEventOverrideAttribute : Attribute
{
	public string? UnrealFunctionName { get; init; }
}


