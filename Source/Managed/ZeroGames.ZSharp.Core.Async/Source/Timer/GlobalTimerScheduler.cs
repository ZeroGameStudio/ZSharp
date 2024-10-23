// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class GlobalTimerScheduler
{

	public static ITimerScheduler Paused => _paused;
	public static ITimerScheduler PausedUnreliable => _pausedUnreliable;
	public static ITimerScheduler Unpaused => _unpaused;
	public static ITimerScheduler Realtime => _realtime;

	static GlobalTimerScheduler()
	{
		IEventLoop.Instance.Register(EEventLoopTickingGroup.DuringWorldTimerTick, static (in EventLoopArgs args, object? _) =>
		{
			_paused.Tick(args.WorldDeltaSeconds);
			_pausedUnreliable.Tick(args.WorldDeltaSeconds);
		}, null);
		
		IEventLoop.Instance.Register(EEventLoopTickingGroup.PostWorldTimerTick, static (in EventLoopArgs args, object? _) =>
		{
			_unpaused.Tick(args.WorldDeltaSeconds);
		}, null);
		
		IEventLoop.Instance.Register(EEventLoopTickingGroup.RealtimeTick, static (in EventLoopArgs args, object? _) =>
		{
			_realtime.Tick(args.RealDeltaSeconds);
		}, null);
	}

	private static ReliableTimerScheduler _paused = new();
	private static UnreliableTimerScheduler _pausedUnreliable = new();
	private static ReliableTimerScheduler _unpaused = new();
	private static ReliableTimerScheduler _realtime = new();

}


