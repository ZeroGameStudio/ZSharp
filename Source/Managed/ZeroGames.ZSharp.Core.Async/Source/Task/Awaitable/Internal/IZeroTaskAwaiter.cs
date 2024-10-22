// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Async.ZeroTask;

// Helps to avoid allocating an Action object when await ZeroTask with AsyncZeroTaskMethodBuilder.
internal interface IZeroTaskAwaiter : INotifyCompletion
{
	void OnCompleted(IAsyncStateMachine stateMachine);
}


