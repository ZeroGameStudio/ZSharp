// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_Memoize<TResult> : ZeroTaskBackendBase<TResult>, IMoveNextSource
{

	public ZeroTaskBackend_Memoize(IZeroTaskBackend backend)
	{
		check(typeof(TResult) == typeof(AsyncVoid));
		check(backend is IReusedZeroTaskBackend);

		ZeroTaskToken token = backend.Token;
		
		_backend = backend;
		_token = token;

		if (backend.GetStatus(token) is EZeroTaskStatus.Pending)
		{
			backend.SetMoveNextSource(this, token);
		}
		else
		{
			try
			{
				_backend.GetResult(_token);
				SetResult(default!);
			}
			catch (Exception ex)
			{
				SetException(ex);
			}
		}
	}

	public ZeroTaskBackend_Memoize(IZeroTaskBackend<TResult> backend)
	{
		check(backend is IReusedZeroTaskBackend);
		
		ZeroTaskToken token = backend.Token;
		
		_backend = backend;
		_token = token;
		
		if (backend.GetStatus(token) is EZeroTaskStatus.Pending)
		{
			backend.SetMoveNextSource(this, token);
		}
		else
		{
			try
			{
				SetResult(backend.GetResult(_token));
			}
			catch (Exception ex)
			{
				SetException(ex);
			}
		}
	}

	void IMoveNextSource.MoveNext()
	{
		try
		{
			if (_backend is IZeroTaskBackend<TResult> typedBackend)
			{
				SetResult(typedBackend.GetResult(_token));
			}
			else
			{
				check(typeof(TResult) == typeof(AsyncVoid));
					
				_backend.GetResult(_token);
				SetResult(default!);
			}
		}
		catch (Exception ex)
		{
			SetException(ex);
		}
	}

	private readonly IZeroTaskBackend _backend;
	private readonly ZeroTaskToken _token;

}


