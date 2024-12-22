// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

/// <summary>
/// Background heap-allocated object for task which completes asynchronously.
/// Similar to IValueTaskSource.
/// </summary>
public interface IZeroTaskBackend
{
	EZeroTaskStatus GetStatus(ZeroTaskToken token);
	void GetResult(ZeroTaskToken token);
	void SetContinuation(Action continuation, ZeroTaskToken token);
	void SetMoveNextSource(IMoveNextSource source, ZeroTaskToken token);
	ZeroTaskToken Token { get; }
}

public interface IZeroTaskBackend<out TResult> : IZeroTaskBackend
{
	new TResult GetResult(ZeroTaskToken token);
}


