// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Async.ZeroTask;

/// <summary>
/// Background heap-allocated object for task which completes asynchronously.
/// Similar to IValueTaskSource.
/// </summary>
public interface IUnderlyingZeroTask
{
	EUnderlyingZeroTaskStatus GetStatus(UnderlyingZeroTaskToken token);
	void GetResult(UnderlyingZeroTaskToken token);
	void SetContinuation(Action continuation, UnderlyingZeroTaskToken token);
	void SetStateMachine(IAsyncStateMachine stateMachine, UnderlyingZeroTaskToken token);
	UnderlyingZeroTaskToken Token { get; }
}

public interface IUnderlyingZeroTask<out TResult> : IUnderlyingZeroTask
{
	new TResult GetResult(UnderlyingZeroTaskToken token);
}


