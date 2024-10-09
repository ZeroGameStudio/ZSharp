// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class)]
public class UClassAttribute : Attribute, IUnrealReflectionSpecifier
{
	public string? Config { get; init; }
	public Type? Within { get; init; }
	[ScanTimeType(nameof(Within))]
	public IScanTimeType? Within_SCANTIME { get; init; }
}


