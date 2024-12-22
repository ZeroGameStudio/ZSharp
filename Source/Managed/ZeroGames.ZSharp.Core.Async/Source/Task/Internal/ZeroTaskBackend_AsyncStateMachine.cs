// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_AsyncStateMachine<TResult, TStateMachine> : PoolableZeroTaskBackendBase<TResult, ZeroTaskBackend_AsyncStateMachine<TResult, TStateMachine>>, IAsyncStateMachineTask<TResult> where TStateMachine : IAsyncStateMachine
{

	public static ZeroTaskBackend_AsyncStateMachine<TResult, TStateMachine> GetFromPool() => Pool.Pop();

	public new void SetResult(TResult result) => base.SetResult(result);
	public new void SetException(Exception exception) => base.SetException(exception);

	public void MoveNext()
	{
		ensure(CanMoveNext);
		
		StateMachine!.MoveNext();
	}

	public TStateMachine? StateMachine { get; set; }
	
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


