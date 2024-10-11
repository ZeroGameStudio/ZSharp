// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintVirtualFunctionSpecifierBase : BlueprintEventSpecifierBase;

public class BlueprintEventAttribute : BlueprintVirtualFunctionSpecifierBase
{
	public string? ImplementationZCallName { get; init; }
}

public class BlueprintEventOverrideAttribute : BlueprintVirtualFunctionSpecifierBase
{
	public required string EventName { get; init; }
}


