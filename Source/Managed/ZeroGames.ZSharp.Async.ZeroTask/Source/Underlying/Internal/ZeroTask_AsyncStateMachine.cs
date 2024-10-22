// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async.ZeroTask;

internal class ZeroTask_AsyncStateMachine<TResult> : UnderlyingZeroTaskBase<TResult, ZeroTask_AsyncStateMachine<TResult>>
{

	public static ZeroTask_AsyncStateMachine<TResult> GetFromPool() => Pool.Pop();

	public void SetResult(TResult result) => Comp.SetResult(result);

	public void SetException(Exception exception) => Comp.SetException(exception);

	public ZeroTask<TResult> Task => new(this);

}


