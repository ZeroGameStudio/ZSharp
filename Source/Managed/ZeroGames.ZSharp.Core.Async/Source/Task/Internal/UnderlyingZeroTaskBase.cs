// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal abstract class UnderlyingZeroTaskBase<TResult, TImpl> : IPoolableUnderlyingZeroTask<TResult, TImpl> where TImpl : UnderlyingZeroTaskBase<TResult, TImpl>, new()
{

	void IPoolableUnderlyingZeroTask<TResult, TImpl>.Initialize()
	{
		_comp.Initialize();
		Initialize();
	}

	void IPoolableUnderlyingZeroTask<TResult, TImpl>.Deinitialize()
	{
		Deinitialize();
		_lifecycle = default;
		_comp.Deinitialize();
	}
	
	public EUnderlyingZeroTaskStatus GetStatus(UnderlyingZeroTaskToken token) => _comp.GetStatus(token);
	public void SetContinuation(Action continuation, UnderlyingZeroTaskToken token) => _comp.SetContinuation(continuation, token);
	public void SetMoveNextSource(IMoveNextSource source, UnderlyingZeroTaskToken token) => _comp.SetMoveNextSource(source, token);
	
	public TResult GetResult(UnderlyingZeroTaskToken token) => _comp.GetResult(token);
	void IUnderlyingZeroTask.GetResult(UnderlyingZeroTaskToken token) => GetResult(token);
	
	public UnderlyingZeroTaskToken Token => _comp.Token;

	TImpl? IPoolableUnderlyingZeroTask<TResult, TImpl>.PoolNext { get; set; }
	
	protected virtual void Initialize(){}
	protected virtual void Deinitialize(){}

	protected void SetResult(TResult result)
	{
		try
		{
			_comp.SetResult(result);
		}
		finally
		{
			ReturnToPool();
		}
	}

	protected void SetException(Exception exception)
	{
		try
		{
			_comp.SetException(exception);
		}
		finally
		{
			ReturnToPool();
		}
	}

	protected static ref UnderlyingZeroTaskPool<TResult, TImpl> Pool => ref _pool;

	protected ref Lifecycle Lifecycle => ref _lifecycle;

	private void ReturnToPool()
	{
		if (!_comp.ResultGot)
		{
			try
			{
				_comp.GetResult(_comp.Token);
			}
			catch (Exception ex)
			{
				UnhandledExceptionHelper.Guard(ex, "Unobserved exception detected in ZeroTask.");
			}
		}
			
		_pool.Push((TImpl)this);
	}
	
	private static UnderlyingZeroTaskPool<TResult, TImpl> _pool;

	private UnderlyingZeroTaskComponent<TResult> _comp;
	private Lifecycle _lifecycle;

}