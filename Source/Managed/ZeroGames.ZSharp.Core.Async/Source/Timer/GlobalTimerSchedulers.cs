// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

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
		IMasterAssemblyLoadContext.RegisterUnloading(Reinitialize, 1);
		Reinitialize();
	}

	[MemberNotNull(nameof(_worldPaused), nameof(_frame), nameof(_worldPausedUnreliable), nameof(_frameUnreliable), nameof(_worldUnpaused), nameof(_realtime))]
	private static void Reinitialize()
	{
		_worldPaused = new(0.001f);
		_frame = new(1);
		
		_worldPausedUnreliable = new(0.001f, false);
		_frameUnreliable = new(1, false);

		_worldUnpaused = new(0.001f);
		_realtime = new(0.001f);
		
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

	private static TimerScheduler<float> _worldPaused;
	private static TimerScheduler<int32> _frame;
	
	private static TimerScheduler<float> _worldPausedUnreliable;
	private static TimerScheduler<int32> _frameUnreliable;
	
	private static TimerScheduler<float> _worldUnpaused;
	private static TimerScheduler<float> _realtime;

}


