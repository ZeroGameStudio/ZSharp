// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public class ReliableTimerScheduler : TimerSchedulerBase
{
	
	public ReliableTimerScheduler(){}

	public ReliableTimerScheduler(Action<Exception> errorHandler) : base(errorHandler){}

	protected override void Advance(TimerData data)
	{
		data.StartTime += data.Rate;
	}

	protected override void BeginTick(){}
	protected override void EndTick(){}

	protected override bool IsOverBudget => false;
	
}


