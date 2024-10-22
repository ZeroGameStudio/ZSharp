// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public readonly struct EventLoopArgs
{
	public required EEventLoopTickingGroup TickingGroup { get; init; }
	public required float WorldDeltaSeconds { get; init; }
	public required float RealDeltaSeconds { get; init; }
	public required double WorldElapsedSeconds { get; init; }
	public required double RealElapsedSeconds { get; init; }
	public required double WorldAccumulatedSeconds { get; init; }
	public required double RealAccumulatedSeconds { get; init; }
}


