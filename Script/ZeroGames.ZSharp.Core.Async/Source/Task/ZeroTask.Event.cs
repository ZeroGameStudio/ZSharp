// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static ZeroTask Until(Func<bool> predicate, TimeSpan pollInterval, Lifecycle lifecycle = default) => throw new NotImplementedException();
	public static ZeroTask Until(Func<bool> predicate, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> Until(predicate, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask Until(Func<bool> predicate, Lifecycle lifecycle = default)
		=> Until(predicate, 0.1f, lifecycle);
	
	public static ZeroTask Until<TState>(Func<TState?, bool> predicate, TState? state, TimeSpan pollInterval, Lifecycle lifecycle = default) => throw new NotImplementedException();
	public static ZeroTask Until<TState>(Func<TState?, bool> predicate, TState? state, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> Until(predicate, state, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask Until<TState>(Func<TState?, bool> predicate, TState? state, Lifecycle lifecycle = default)
		=> Until(predicate, state, 0.1f, lifecycle);
	
	public static ZeroTask<T> UntilValueChanged<T>(Func<T> selector, TimeSpan pollInterval, Lifecycle lifecycle = default) => throw new NotImplementedException();
	public static ZeroTask<T> UntilValueChanged<T>(Func<T> selector, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask<T> UntilValueChanged<T>(Func<T> selector, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, 0.1f, lifecycle);
	
	public static ZeroTask<T> UntilValueChanged<T, TState>(Func<TState?, T> selector, TState? state, TimeSpan pollInterval, Lifecycle lifecycle = default) => throw new NotImplementedException();
	public static ZeroTask<T> UntilValueChanged<T, TState>(Func<TState?, T> selector, TState? state, float pollIntervalSeconds, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, state, TimeSpan.FromSeconds(pollIntervalSeconds), lifecycle);
	public static ZeroTask<T> UntilValueChanged<T, TState>(Func<TState?, T> selector, TState? state, Lifecycle lifecycle = default)
		=> UntilValueChanged(selector, state, 0.1f, lifecycle);
	
}


