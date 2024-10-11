// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
public class DefaultSubobjectClassOverrideAttribute : Attribute, IUnrealReflectionSpecifier
{
	public required string Subobject { get; init; }
	public required Type Class { get; init; }
	[ScanTimeType(nameof(Class))]
	public IScanTimeType Class_SCANTIME { get; init; } = null!;
}


