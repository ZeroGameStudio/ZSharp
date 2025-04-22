// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static async ZeroTask Until(Func<bool> predicate, TimeSpan pollInterval, Lifecycle lifecycle = default)
	{
		while (!predicate())
		{
			await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifecycle);
		}
	}
	public static ZeroTask Until(Func<bool> predicate, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> Until(predicate, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask Until(Func<bool> predicate, Lifecycle lifecycle = default)
		=> Until(predicate, 0.1f, lifecycle);

	public static async ZeroTask Until<TState>(Func<TState, bool> predicate, TState state, TimeSpan pollInterval, Lifecycle lifecycle = default)
	{
		while (!predicate(state))
		{
			await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifecycle);
		}
	}
	public static ZeroTask Until<TState>(Func<TState, bool> predicate, TState state, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> Until(predicate, state, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask Until<TState>(Func<TState, bool> predicate, TState state, Lifecycle lifecycle = default)
		=> Until(predicate, state, 0.1f, lifecycle);

	public static async ZeroTask<T> UntilValueChanged<T>(Func<T> selector, IEqualityComparer<T> comparer, TimeSpan pollInterval, Lifecycle lifecycle = default)
	{
		T last = selector();
		T current = last;
		do
		{
			await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifecycle);
			last = current;
			current = selector();
		} while (comparer.Equals(last, current));
		return current;
	}
	public static ZeroTask<T> UntilValueChanged<T>(Func<T> selector, IEqualityComparer<T> comparer, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, comparer, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask<T> UntilValueChanged<T>(Func<T> selector, IEqualityComparer<T> comparer, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, comparer, 0.1f, lifecycle);

	public static async ZeroTask<T> UntilValueChanged<T, TState>(Func<TState, T> selector, IEqualityComparer<T> comparer, TState state, TimeSpan pollInterval, Lifecycle lifecycle = default)
	{
		{
			T last = selector(state);
			T current = last;
			do
			{
				await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifecycle);
				last = current;
				current = selector(state);
			} while (comparer.Equals(last, current));
			return current;
		}
	}
	public static ZeroTask<T> UntilValueChanged<T, TState>(Func<TState, T> selector, IEqualityComparer<T> comparer, TState state, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, comparer, state, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask<T> UntilValueChanged<T, TState>(Func<TState, T> selector, IEqualityComparer<T> comparer, TState state, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, comparer, state, 0.1f, lifecycle);
	
}


