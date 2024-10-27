// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public abstract class UnderlyingZeroTaskBase<TResult, TImpl> : IPoolableUnderlyingZeroTask<TResult, TImpl> where TImpl : class, IPoolableUnderlyingZeroTask<TResult, TImpl>, new()
{

	public UnderlyingZeroTaskBase()
	{
		check(GetType() == typeof(TImpl));
	}

	void IPoolableUnderlyingZeroTask<TResult, TImpl>.Initialize() => _comp.Initialize();
	void IPoolableUnderlyingZeroTask<TResult, TImpl>.Deinitialize() => _comp.Deinitialize();
	public EUnderlyingZeroTaskStatus GetStatus(UnderlyingZeroTaskToken token) => _comp.GetStatus(token);
	public void SetContinuation(Action continuation, UnderlyingZeroTaskToken token) => _comp.SetContinuation(continuation, token);
	public void SetStateMachine(IAsyncStateMachine stateMachine, UnderlyingZeroTaskToken token) => _comp.SetStateMachine(stateMachine, token);
	
	void IUnderlyingZeroTask.GetResult(UnderlyingZeroTaskToken token) => GetResult(token);
	public TResult GetResult(UnderlyingZeroTaskToken token)
	{
		TResult result = _comp.GetResult(token);
		_pool.Push(_impl);
		return result;
	}

	public UnderlyingZeroTaskToken Token => _comp.Token;

	TImpl? IPoolableUnderlyingZeroTask<TResult, TImpl>.PoolNext { get; set; }

	protected static ref UnderlyingZeroTaskPool<TResult, TImpl> Pool => ref _pool;

	protected ref UnderlyingZeroTaskComponent<TResult> Comp => ref _comp;
	protected ref Lifecycle Lifecycle => ref _lifecycle;
	
	protected bool ShouldThrowOnLifecycleExpired { get; set; }

	private TImpl _impl => Unsafe.As<TImpl>(this);
	
	private static UnderlyingZeroTaskPool<TResult, TImpl> _pool;

	private UnderlyingZeroTaskComponent<TResult> _comp;
	private Lifecycle _lifecycle;

}