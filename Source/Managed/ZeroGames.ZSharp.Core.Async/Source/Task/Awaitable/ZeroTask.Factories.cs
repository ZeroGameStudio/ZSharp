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

	public static ZeroTask<float> Delay(EZeroTaskDelayType delayType, TimeSpan delayTime, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();

		ZeroTask_Delay delay = ZeroTask_Delay.GetFromPool(delayType, delayTime.Seconds, lifecycle);
		ZeroTask<float> task = FromUnderlyingTask(delay);
		delay.Run();
		return task;
	}
	public static ZeroTask<float> Delay(float delaySeconds, Lifecycle lifecycle = default) => Delay(TimeSpan.FromSeconds(delaySeconds), lifecycle);
	public static ZeroTask<float> Delay(TimeSpan delayTime, Lifecycle lifecycle = default) => Delay(EZeroTaskDelayType.WorldPaused, delayTime, lifecycle);
	public static ZeroTask<float> UnpausedDelay(float delaySeconds, Lifecycle lifecycle = default) => UnpausedDelay(TimeSpan.FromSeconds(delaySeconds), lifecycle);
	public static ZeroTask<float> UnpausedDelay(TimeSpan delayTime, Lifecycle lifecycle = default) => Delay(EZeroTaskDelayType.WorldUnpaused, delayTime, lifecycle);

	public static ZeroTask Yield() => throw new NotImplementedException();

	public static ZeroTask CompletedTask => default;

}


