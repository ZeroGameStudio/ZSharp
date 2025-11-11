// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static async ZeroTask Until(Func<bool> predicate, TimeSpan pollInterval, Lifetime lifetime = default)
	{
		while (!predicate())
		{
			await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifetime);
		}
	}
	public static ZeroTask Until(Func<bool> predicate, float pollIntervalSeconds, Lifetime lifetime = default)
		=> Until(predicate, TimeSpan.FromSeconds(pollIntervalSeconds), lifetime);
	public static ZeroTask Until(Func<bool> predicate, Lifetime lifetime = default)
		=> Until(predicate, 0.1f, lifetime);

	public static async ZeroTask Until<TState>(Func<TState, bool> predicate, TState state, TimeSpan pollInterval, Lifetime lifetime = default)
	{
		while (!predicate(state))
		{
			await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifetime);
		}
	}
	public static ZeroTask Until<TState>(Func<TState, bool> predicate, TState state, float pollIntervalSeconds, Lifetime lifetime = default)
		=> Until(predicate, state, TimeSpan.FromSeconds(pollIntervalSeconds), lifetime);
	public static ZeroTask Until<TState>(Func<TState, bool> predicate, TState state, Lifetime lifetime = default)
		=> Until(predicate, state, 0.1f, lifetime);

	public static async ZeroTask<T> UntilValueChanged<T>(Func<T> selector, IEqualityComparer<T> comparer, TimeSpan pollInterval, Lifetime lifetime = default)
	{
		T last = selector();
		T current = last;
		do
		{
			await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifetime);
			last = current;
			current = selector();
		} while (comparer.Equals(last, current));
		return current;
	}
	public static ZeroTask<T> UntilValueChanged<T>(Func<T> selector, IEqualityComparer<T> comparer, float pollIntervalSeconds, Lifetime lifetime = default)
		=> UntilValueChanged(selector, comparer, TimeSpan.FromSeconds(pollIntervalSeconds), lifetime);
	public static ZeroTask<T> UntilValueChanged<T>(Func<T> selector, IEqualityComparer<T> comparer, Lifetime lifetime = default)
		=> UntilValueChanged(selector, comparer, 0.1f, lifetime);

	public static async ZeroTask<T> UntilValueChanged<T, TState>(Func<TState, T> selector, IEqualityComparer<T> comparer, TState state, TimeSpan pollInterval, Lifetime lifetime = default)
	{
		{
			T last = selector(state);
			T current = last;
			do
			{
				await Delay(EZeroTaskDelayType.Realtime, pollInterval, lifetime);
				last = current;
				current = selector(state);
			} while (comparer.Equals(last, current));
			return current;
		}
	}
	public static ZeroTask<T> UntilValueChanged<T, TState>(Func<TState, T> selector, IEqualityComparer<T> comparer, TState state, float pollIntervalSeconds, Lifetime lifetime = default)
		=> UntilValueChanged(selector, comparer, state, TimeSpan.FromSeconds(pollIntervalSeconds), lifetime);
	public static ZeroTask<T> UntilValueChanged<T, TState>(Func<TState, T> selector, IEqualityComparer<T> comparer, TState state, Lifetime lifetime = default)
		=> UntilValueChanged(selector, comparer, state, 0.1f, lifetime);
	
}


