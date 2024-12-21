// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTask_AsyncStateMachine<TResult, TStateMachine> : UnderlyingZeroTaskBase<TResult, ZeroTask_AsyncStateMachine<TResult, TStateMachine>>, IAsyncStateMachineTask<TResult> where TStateMachine : IAsyncStateMachine
{

	public static ZeroTask_AsyncStateMachine<TResult, TStateMachine> GetFromPool() => Pool.Pop();

	public new void SetResult(TResult result) => base.SetResult(result);
	public new void SetException(Exception exception) => base.SetException(exception);

	public void MoveNext() => StateMachine!.MoveNext();

	public TStateMachine? StateMachine { get; set; }
	
	public Action MoveNextDelegate
	{
		get
		{
			_moveNextDelegate ??= () =>
			{
				if (IsInGameThread || !AsyncSettings.ForceAsyncZeroTaskMethodContinueOnGameThread)
				{
					MoveNext();
				}
				else
				{
					GameThreadScheduler.Instance.Post(state => ((IMoveNextSource)state!).MoveNext(), this);
				}
			};
			
			return _moveNextDelegate;
		}
	}

	private Action? _moveNextDelegate;

}


