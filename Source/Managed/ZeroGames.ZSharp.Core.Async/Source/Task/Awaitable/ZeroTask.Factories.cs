// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ZeroTask
{

	public static ZeroTask<T> FromResult<T>(T result)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(result);
	}

	public static ZeroTask FromUnderlyingTask(IUnderlyingZeroTask task)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(task);
	}

	public static ZeroTask<T> FromUnderlyingTask<T>(IUnderlyingZeroTask<T> task)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(task);
	}
	
	public static ZeroTask FromException(Exception exception) => throw new NotImplementedException();
	
	public static ZeroTask<T> FromException<T>(Exception exception) => throw new NotImplementedException();

	public static ZeroTask<TimeSpan> Delay(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (delayTime < TimeSpan.FromMilliseconds(1))
		{
			delayTime = TimeSpan.FromMilliseconds(1);
		}
		
		ZeroTask_Delay delay = ZeroTask_Delay.GetFromPool(delayType, delayTime, lifecycle);
		ZeroTask<TimeSpan> task = FromUnderlyingTask(delay);
		delay.Run();
		return task;
	}
	public static ZeroTask<TimeSpan> Delay(double delayTimeMs, Lifecycle lifecycle = default) => Delay(TimeSpan.FromMilliseconds(delayTimeMs), lifecycle);
	public static ZeroTask<TimeSpan> Delay(TimeSpan delayTime, Lifecycle lifecycle = default) => Delay(EZeroTaskDelayType.Paused, delayTime, lifecycle);
	public static ZeroTask<TimeSpan> UnpausedDelay(double delayTimeMs, Lifecycle lifecycle = default) => UnpausedDelay(TimeSpan.FromMilliseconds(delayTimeMs), lifecycle);
	public static ZeroTask<TimeSpan> UnpausedDelay(TimeSpan delayTime, Lifecycle lifecycle = default) => Delay(EZeroTaskDelayType.Unpaused, delayTime, lifecycle);

	public static ZeroTask Yield() => throw new NotImplementedException();

	public static ZeroTask CompletedTask => default;

}


