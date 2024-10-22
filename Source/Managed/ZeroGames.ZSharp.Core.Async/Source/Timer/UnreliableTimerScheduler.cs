// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.Async;

public class UnreliableTimerScheduler : TimerSchedulerBase
{
	
	public UnreliableTimerScheduler(){}

	public UnreliableTimerScheduler(Action<Exception> errorHandler) : base(errorHandler){}

	public TimeSpan TimeBudget { get; set; } = TimeSpan.FromMilliseconds(1);
	
	protected override void Advance(TimerData data)
	{
		data.StartTime = AccumulatedTime;
	}

	protected override void BeginTick()
	{
		_stopwatch.Restart();
	}

	protected override void EndTick()
	{
		_stopwatch.Stop();
	}

	protected override bool IsOverBudget => TimeBudget > TimeSpan.Zero && TimeSpan.FromTicks(_stopwatch.ElapsedTicks) > TimeBudget;

	private Stopwatch _stopwatch = new();

}


