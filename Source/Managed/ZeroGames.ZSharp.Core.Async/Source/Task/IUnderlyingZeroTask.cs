// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

/// <summary>
/// Background heap-allocated object for task which completes asynchronously.
/// Similar to IValueTaskSource.
/// </summary>
public interface IUnderlyingZeroTask
{
	EUnderlyingZeroTaskStatus GetStatus(UnderlyingZeroTaskToken token);
	void GetResult(UnderlyingZeroTaskToken token);
	void SetContinuation(Action continuation, UnderlyingZeroTaskToken token);
	void SetMoveNextSource(IMoveNextSource source, UnderlyingZeroTaskToken token);
	UnderlyingZeroTaskToken Token { get; }
}

public interface IUnderlyingZeroTask<out TResult> : IUnderlyingZeroTask
{
	new TResult GetResult(UnderlyingZeroTaskToken token);
}


