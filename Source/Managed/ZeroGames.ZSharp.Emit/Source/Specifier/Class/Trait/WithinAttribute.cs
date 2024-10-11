// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class)]
public class WithinAttribute(Type outerType) : Attribute, IUnrealReflectionSpecifier
{
	public Type OuterType { get; } = outerType;
	[ScanTimeType(0)]
	public IScanTimeType OuterType_SCANTIME { get; init; } = null!;
}


