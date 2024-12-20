// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class GlobalTimerSchedulers
{

	public static ITimerScheduler<float> WorldPaused => _worldPaused;
	public static ITimerScheduler<int32> Frame => _frame;
	
	public static ITimerScheduler<float> WorldPausedUnreliable => _worldPausedUnreliable;
	public static ITimerScheduler<int32> FrameUnreliable => _frameUnreliable;
	
	public static ITimerScheduler<float> WorldUnpaused => _worldUnpaused;
	public static ITimerScheduler<float> Realtime => _realtime;

	static GlobalTimerSchedulers()
	{
		IEventLoop.Instance.Register(EEventLoopTickingGroup.DuringWorldTimerTick, static (in EventLoopArgs args, ref bool _) =>
		{
			_worldPaused.Tick(args.WorldDeltaSeconds);
			_frame.Tick(1);
			
			_worldPausedUnreliable.Tick(args.WorldDeltaSeconds);
			_frameUnreliable.Tick(1);
		});
		
		IEventLoop.Instance.Register(EEventLoopTickingGroup.PostWorldTimerTick, static (in EventLoopArgs args, ref bool _) =>
		{
			_worldUnpaused.Tick(args.WorldDeltaSeconds);
		});
		
		IEventLoop.Instance.Register(EEventLoopTickingGroup.RealtimeTick, static (in EventLoopArgs args, ref bool _) =>
		{
			_realtime.Tick(args.RealDeltaSeconds);
		});
	}

	private static readonly TimerScheduler<float> _worldPaused = new(0.001f);
	private static readonly TimerScheduler<int32> _frame = new(1);
	
	private static readonly TimerScheduler<float> _worldPausedUnreliable = new(0.001f, false);
	private static readonly TimerScheduler<int32> _frameUnreliable = new(1, false);
	
	private static readonly TimerScheduler<float> _worldUnpaused = new(0.001f);
	private static readonly TimerScheduler<float> _realtime = new(0.001f);

}


