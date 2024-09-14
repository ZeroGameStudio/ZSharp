// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class BlueprintEventOverrideAttribute() : Attribute
{
	public BlueprintEventOverrideAttribute(string unrealFunctionName) : this() => UnrealFunctionName = unrealFunctionName;
	public string? UnrealFunctionName { get; }
}


