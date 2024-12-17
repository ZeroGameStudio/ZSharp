// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTask_AsyncStateMachine<TResult> : UnderlyingZeroTaskBase<TResult, ZeroTask_AsyncStateMachine<TResult>>
{

	public static ZeroTask_AsyncStateMachine<TResult> GetFromPool() => Pool.Pop();

	public new void SetResult(TResult result) => base.SetResult(result);
	public new void SetException(Exception exception) => base.SetException(exception);

	public ZeroTask<TResult> Task => new(this);

}


