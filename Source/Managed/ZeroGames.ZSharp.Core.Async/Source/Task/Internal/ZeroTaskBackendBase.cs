// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal abstract class ZeroTaskBackendBase<TResult, TImpl> : IPoolableZeroTaskBackend<TResult, TImpl> where TImpl : ZeroTaskBackendBase<TResult, TImpl>, new()
{

	void IPoolableZeroTaskBackend<TResult, TImpl>.Initialize()
	{
		_comp.Initialize();
		Initialize();
	}

	void IPoolableZeroTaskBackend<TResult, TImpl>.Deinitialize()
	{
		Deinitialize();
		_lifecycle = default;
		_comp.Deinitialize();
	}
	
	public EZeroTaskStatus GetStatus(ZeroTaskToken token) => _comp.GetStatus(token);
	public void SetContinuation(Action continuation, ZeroTaskToken token) => _comp.SetContinuation(continuation, token);
	public void SetMoveNextSource(IMoveNextSource source, ZeroTaskToken token) => _comp.SetMoveNextSource(source, token);
	
	public TResult GetResult(ZeroTaskToken token) => _comp.GetResult(token);
	void IZeroTaskBackend.GetResult(ZeroTaskToken token) => GetResult(token);
	
	public ZeroTaskToken Token => _comp.Token;

	TImpl? IPoolableZeroTaskBackend<TResult, TImpl>.PoolNext { get; set; }
	
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

	protected static ref ZeroTaskBackendPool<TResult, TImpl> Pool => ref _pool;

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
	
	private static ZeroTaskBackendPool<TResult, TImpl> _pool;

	private ZeroTaskBackendComp<TResult> _comp;
	private Lifecycle _lifecycle;

}