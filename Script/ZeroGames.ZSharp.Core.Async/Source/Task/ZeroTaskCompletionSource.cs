// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct ZeroTaskCompletionSource
{

	public static ZeroTaskCompletionSource Create(Lifecycle lifecycle = default)
		=> new(ZeroTaskBackend_CompletionSource<AsyncVoid>.GetFromPool(lifecycle));
	
	public static ZeroTaskCompletionSource<TResult> Create<TResult>(Lifecycle lifecycle = default)
		=> new(ZeroTaskBackend_CompletionSource<TResult>.GetFromPool(lifecycle));
	
	public void SetResult() => _backend.SetResult(default, _token);
	public void SetException(Exception exception) => _backend.SetException(exception, _token);

	public ZeroTask Task => _backend.GetTask(_token);

	private ZeroTaskCompletionSource(ZeroTaskBackend_CompletionSource<AsyncVoid> backend)
	{
		_backend = backend;
		_token = backend.Token;
	}

	private readonly ZeroTaskBackend_CompletionSource<AsyncVoid> _backend;
	private readonly ZeroTaskToken _token;
	
}

public readonly struct ZeroTaskCompletionSource<TResult>
{
	
	public void SetResult(TResult result) => _backend.SetResult(result, _token);
	public void SetException(Exception exception) => _backend.SetException(exception, _token);

	public ZeroTask<TResult> Task => _backend.GetTask(_token);

	internal ZeroTaskCompletionSource(ZeroTaskBackend_CompletionSource<TResult> backend)
	{
		_backend = backend;
		_token = backend.Token;
	}

	private readonly ZeroTaskBackend_CompletionSource<TResult> _backend;
	private readonly ZeroTaskToken _token;
	
}


