﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
public class PropertyDefaultOverrideAttribute : ClassSpecifierBase
{
	public required string Property { get; init; }
	public required object Default { get; init; }
}

