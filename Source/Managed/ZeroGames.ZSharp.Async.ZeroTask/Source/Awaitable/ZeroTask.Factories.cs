// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async.ZeroTask;

public partial struct ZeroTask
{

	public static ZeroTask<T> FromResult<T>(T result)
	{
		ThreadHelper.ValidateGameThread();

		return new(result);
	}

	public static ZeroTask FromUnderlyingTask(IUnderlyingZeroTask task)
	{
		ThreadHelper.ValidateGameThread();

		return new(task);
	}

	public static ZeroTask<T> FromUnderlyingTask<T>(IUnderlyingZeroTask<T> task)
	{
		ThreadHelper.ValidateGameThread();

		return new(task);
	}
	
	public static ZeroTask FromException(Exception exception) => throw new NotImplementedException();
	
	public static ZeroTask<T> FromException<T>(Exception exception) => throw new NotImplementedException();

	public static ZeroTask<TimeSpan> Delay(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle = default, bool throwOnExpired = false)
	{
		ThreadHelper.ValidateGameThread();
		
		if (delayTime <= TimeSpan.Zero)
		{
			return FromResult(TimeSpan.Zero);
		}
		
		ZeroTask_Delay delay = ZeroTask_Delay.GetFromPool(delayType, delayTime, lifecycle, throwOnExpired);
		ZeroTask<TimeSpan> task = FromUnderlyingTask(delay);
		delay.Run();
		return task;
	}
	public static ZeroTask<TimeSpan> Delay(double delayTimeMs, Lifecycle lifecycle = default, bool throwOnExpired = false) => Delay(TimeSpan.FromMilliseconds(delayTimeMs), lifecycle, throwOnExpired);
	public static ZeroTask<TimeSpan> Delay(TimeSpan delayTime, Lifecycle lifecycle = default, bool throwOnExpired = false) => Delay(EZeroTaskDelayType.WorldPaused, delayTime, lifecycle, throwOnExpired);
	public static ZeroTask<TimeSpan> UnpausedDelay(double delayTimeMs, Lifecycle lifecycle = default, bool throwOnExpired = false) => UnpausedDelay(TimeSpan.FromMilliseconds(delayTimeMs), lifecycle, throwOnExpired);
	public static ZeroTask<TimeSpan> UnpausedDelay(TimeSpan delayTime, Lifecycle lifecycle = default, bool throwOnExpired = false) => Delay(EZeroTaskDelayType.WorldUnpaused, delayTime, lifecycle, throwOnExpired);
	public static ZeroTask<TimeSpan> RealDelay(double delayTimeMs, Lifecycle lifecycle = default, bool throwOnExpired = false) => Delay(TimeSpan.FromMilliseconds(delayTimeMs), lifecycle, throwOnExpired);
	public static ZeroTask<TimeSpan> RealDelay(TimeSpan delayTime, Lifecycle lifecycle = default, bool throwOnExpired = false) => Delay(EZeroTaskDelayType.RealPaused, delayTime, lifecycle, throwOnExpired);
	public static ZeroTask<TimeSpan> RealUnpausedDelay(double delayTimeMs, Lifecycle lifecycle = default, bool throwOnExpired = false) => Delay(TimeSpan.FromMilliseconds(delayTimeMs), lifecycle, throwOnExpired);
	public static ZeroTask<TimeSpan> RealUnpausedDelay(TimeSpan delayTime, Lifecycle lifecycle = default, bool throwOnExpired = false) => Delay(EZeroTaskDelayType.RealUnpaused, delayTime, lifecycle, throwOnExpired);

	public static ZeroTask Yield() => throw new NotImplementedException();

	public static ZeroTask CompletedTask => default;

}


