// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal interface IAsyncStateMachineTask<in TResult> : IMoveNextSource
{
	void SetResult(TResult result);
	void SetException(Exception exception);
	
	Action MoveNextDelegate { get; }
}


