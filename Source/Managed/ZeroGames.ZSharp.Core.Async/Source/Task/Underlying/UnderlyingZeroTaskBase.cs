// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public abstract class UnderlyingZeroTaskBase<TResult, TImpl> : IPoolableUnderlyingZeroTask<TResult, TImpl> where TImpl : UnderlyingZeroTaskBase<TResult, TImpl>, new()
{

	void IPoolableUnderlyingZeroTask<TResult, TImpl>.Initialize() => _comp.Initialize();
	void IPoolableUnderlyingZeroTask<TResult, TImpl>.Deinitialize() => _comp.Deinitialize();
	public EUnderlyingZeroTaskStatus GetStatus(UnderlyingZeroTaskToken token) => _comp.GetStatus(token);
	public void SetContinuation(Action continuation, UnderlyingZeroTaskToken token) => _comp.SetContinuation(continuation, token);
	public void SetStateMachine(IAsyncStateMachine stateMachine, UnderlyingZeroTaskToken token) => _comp.SetStateMachine(stateMachine, token);
	
	public TResult GetResult(UnderlyingZeroTaskToken token)
	{
		TResult result = _comp.GetResult(token);
		_pool.Push((TImpl)this);
		return result;
	}
	void IUnderlyingZeroTask.GetResult(UnderlyingZeroTaskToken token) => GetResult(token);
	
	protected void SetResult(TResult result) => _comp.SetResult(result);
	protected void SetException(Exception exception) => _comp.SetException(exception);

	public UnderlyingZeroTaskToken Token => _comp.Token;

	TImpl? IPoolableUnderlyingZeroTask<TResult, TImpl>.PoolNext { get; set; }

	protected static ref UnderlyingZeroTaskPool<TResult, TImpl> Pool => ref _pool;

	protected ref Lifecycle Lifecycle => ref _lifecycle;
	
	protected bool ShouldThrowOnLifecycleExpired { get; set; }

	private static UnderlyingZeroTaskPool<TResult, TImpl> _pool;

	private UnderlyingZeroTaskComponent<TResult> _comp;
	private Lifecycle _lifecycle;

}