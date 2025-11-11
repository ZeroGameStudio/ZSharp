// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal abstract class ZeroTaskBackendBase<TResult> : IZeroTaskBackend<TResult>
{

	public ZeroTaskBackendBase()
	{
		_comp.Initialize();
	}
	
	public EZeroTaskStatus GetStatus(ZeroTaskToken token) => _comp.GetStatus(token);
	public void SetContinuation(Action continuation, ZeroTaskToken token) => _comp.SetContinuation(continuation, token);
	public void SetMoveNextSource(IMoveNextSource source, ZeroTaskToken token) => _comp.SetMoveNextSource(source, token);

	public TResult GetResult(ZeroTaskToken token)
	{
		TResult result = _comp.GetResult(token);
		PostGetResult();
		GC.SuppressFinalize(this);
		return result;
	}
	void IZeroTaskBackend.GetResult(ZeroTaskToken token) => GetResult(token);

	public bool IsCompletedEvenIfRecycled(ZeroTaskToken token)
	{
		return Token != token || GetStatus(token) != EZeroTaskStatus.Pending;
	}
	
	public ZeroTaskToken Token => _comp.Token;

	protected virtual void PostGetResult(){}
	protected void SetResult(TResult result) => _comp.SetResult(result);
	protected void SetException(Exception exception) => _comp.SetException(exception);
	protected void TrySetResult(TResult result) => _comp.TrySetResult(result);
	protected void TrySetException(Exception exception) => _comp.TrySetException(exception);
	
	protected ref Lifetime Lifetime => ref _lifetime;
	protected ref ZeroTaskBackendComp<TResult> Comp => ref _comp;

	~ZeroTaskBackendBase() => Comp.TryPublishUnobservedException();
	
	private ZeroTaskBackendComp<TResult> _comp;
	private Lifetime _lifetime;
	
}


