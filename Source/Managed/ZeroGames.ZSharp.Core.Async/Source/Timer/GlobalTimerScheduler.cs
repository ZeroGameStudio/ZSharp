// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public static class GlobalTimerScheduler
{

	public static ITimerScheduler WorldPausedReliable => _worldPausedReliable;
	public static ITimerScheduler WorldPausedUnreliable => _worldPausedUnreliable;
	public static ITimerScheduler WorldUnpausedReliable => _worldUnpausedReliable;
	public static ITimerScheduler WorldUnpausedUnreliable => _worldUnpausedUnreliable;
	public static ITimerScheduler RealPausedReliable => _realPausedReliable;
	public static ITimerScheduler RealPausedUnreliable => _realPausedUnreliable;
	public static ITimerScheduler RealUnpausedReliable => _realUnpausedReliable;
	public static ITimerScheduler RealUnpausedUnreliable => _realUnpausedUnreliable;

	static GlobalTimerScheduler()
	{
		IEventLoop.Instance.Register(EEventLoopTickingGroup.DuringWorldTimerTick, static (in EventLoopArgs args, object? _) =>
		{
			_worldPausedReliable.Tick(args.WorldDeltaSeconds);
			_worldPausedUnreliable.Tick(args.WorldDeltaSeconds);
			_realPausedReliable.Tick(args.RealDeltaSeconds);
			_realPausedUnreliable.Tick(args.RealDeltaSeconds);
		}, null);
		
		IEventLoop.Instance.Register(EEventLoopTickingGroup.PostWorldTimerTick, static (in EventLoopArgs args, object? _) =>
		{
			_worldUnpausedReliable.Tick(args.WorldDeltaSeconds);
			_worldUnpausedUnreliable.Tick(args.WorldDeltaSeconds);
			_realUnpausedReliable.Tick(args.RealDeltaSeconds);
			_realUnpausedUnreliable.Tick(args.RealDeltaSeconds);
		}, null);
	}

	private static ReliableTimerScheduler _worldPausedReliable = new();
	private static UnreliableTimerScheduler _worldPausedUnreliable = new();
	private static ReliableTimerScheduler _worldUnpausedReliable = new();
	private static UnreliableTimerScheduler _worldUnpausedUnreliable = new();
	private static ReliableTimerScheduler _realPausedReliable = new();
	private static UnreliableTimerScheduler _realPausedUnreliable = new();
	private static ReliableTimerScheduler _realUnpausedReliable = new();
	private static UnreliableTimerScheduler _realUnpausedUnreliable = new();

}


