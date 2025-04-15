// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class ClampRangeAttribute : PropertySpecifierBase
{
	public double Min { get; init; } = double.NegativeInfinity;
	public double Max { get; init; } = double.PositiveInfinity;
}


