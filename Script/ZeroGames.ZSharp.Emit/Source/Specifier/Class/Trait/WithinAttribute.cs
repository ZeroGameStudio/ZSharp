// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class WithinAttribute(Type outerType) : ClassSpecifierBase
{
	public Type OuterType { get; } = outerType;
	[ScanTimeType(0)]
	public IScanTimeType OuterType_SCANTIME { get; init; } = null!;
}


