﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintEventSpecifierBase : EventSpecifierBase;

public class BlueprintEventAttribute : BlueprintEventSpecifierBase
{
	public string? Implementation { get; init; }
}

public class BlueprintEventOverrideAttribute : BlueprintEventSpecifierBase
{
	public required string Event { get; init; }
}

