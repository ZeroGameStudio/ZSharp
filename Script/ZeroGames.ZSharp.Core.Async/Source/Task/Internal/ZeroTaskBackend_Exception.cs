// Copyright Zero Games. All Rights Reserved.

using System.Runtime.ExceptionServices;

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_Exception<TResult> : IZeroTaskBackend<TResult>
{

	public ZeroTaskBackend_Exception(Exception exception)
	{
		_exception = ExceptionDispatchInfo.Capture(exception);
		_status = exception is OperationCanceledException ? EZeroTaskStatus.Canceled : EZeroTaskStatus.Faulted;
	}

	public EZeroTaskStatus GetStatus(ZeroTaskToken token) => _status;
	public TResult GetResult(ZeroTaskToken token)
	{
		_exception.Throw();
		return default;
	}
	void IZeroTaskBackend.GetResult(ZeroTaskToken token) => GetResult(token);
	void IZeroTaskBackend.SetContinuation(Action continuation, ZeroTaskToken token) => throw new NotSupportedException();
	void IZeroTaskBackend.SetMoveNextSource(IMoveNextSource source, ZeroTaskToken token) => throw new NotSupportedException();

	ZeroTaskToken IZeroTaskBackend.Token { get; } = default(ZeroTaskToken).Next;
	
	private readonly ExceptionDispatchInfo _exception;
	private readonly EZeroTaskStatus _status;

}


