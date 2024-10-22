// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

// Helps to avoid allocating an Action object when await ZeroTask with AsyncZeroTaskMethodBuilder.
internal interface IZeroTaskAwaiter : INotifyCompletion
{
	void OnCompleted(IAsyncStateMachine stateMachine);
}


