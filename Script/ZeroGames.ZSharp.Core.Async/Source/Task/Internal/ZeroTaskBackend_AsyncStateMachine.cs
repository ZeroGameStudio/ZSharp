// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_AsyncStateMachine<TResult, TStateMachine> : PooledZeroTaskBackendBase<TResult, ZeroTaskBackend_AsyncStateMachine<TResult, TStateMachine>>, IAsyncStateMachineTask<TResult> where TStateMachine : IAsyncStateMachine
{

	public static ZeroTaskBackend_AsyncStateMachine<TResult, TStateMachine> GetFromPool() => InternalGetFromPool();

	public new void SetResult(TResult result) => base.SetResult(result);
	public new void SetException(Exception exception) => base.SetException(exception);
	public void GetResult() => base.GetResult(Token);

	public void MoveNext()
	{
		ensure(CanMoveNext);
		
		StateMachine!.MoveNext();
	}

	// Use field to ensure mutable and avoid copy.
	public TStateMachine? StateMachine;
	
	public Action MoveNextDelegate
	{
		get
		{
			_moveNextDelegate ??= () =>
			{
				if (CanMoveNext)
				{
					MoveNext();
				}
				else
				{
					GameThreadScheduler.Instance.Post(static state => ((IMoveNextSource)state!).MoveNext(), this);
				}
			};
			
			return _moveNextDelegate;
		}
	}

	private bool CanMoveNext => IsInGameThread || !AsyncSettings.ForceAsyncZeroTaskMethodContinueOnGameThread;
	
	private Action? _moveNextDelegate;

}


