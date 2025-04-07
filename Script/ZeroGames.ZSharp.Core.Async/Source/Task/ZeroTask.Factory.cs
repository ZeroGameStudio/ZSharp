// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static ZeroTask<T> FromResult<T>(T result)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(result);
	}
	
	public static ZeroTask FromException(Exception exception) => FromException<AsyncVoid>(exception);
	public static ZeroTask<T> FromException<T>(Exception exception)
	{
		Thrower.ThrowIfNotInGameThread();

		return FromBackend(new ZeroTaskBackend_Exception<T>(exception));
	}

	public static ZeroTask FromExpired(Lifecycle lifecycle) => FromExpired<AsyncVoid>(lifecycle);
	public static ZeroTask<T> FromExpired<T>(Lifecycle lifecycle)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (!lifecycle.IsExpired)
		{
			throw new ArgumentOutOfRangeException(nameof(lifecycle));
		}
		
		return FromException<T>(new LifecycleExpiredException(lifecycle));
	}
	
	public static ZeroTask FromBackend(IZeroTaskBackend backend, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(backend);
	}

	public static ZeroTask<T> FromBackend<T>(IZeroTaskBackend<T> backend, Lifecycle lifecycle = default)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(backend);
	}

	public static ZeroTask FromCompletionSource(out ZeroTaskCompletionSource source, Lifecycle lifecycle = default)
	{
		source = ZeroTaskCompletionSource.Create();
		return source.Task;
	}

	public static ZeroTask<T> FromCompletionSource<T>(out ZeroTaskCompletionSource<T> source, Lifecycle lifecycle = default)
	{
		source = ZeroTaskCompletionSource<T>.Create();
		return source.Task;
	}
	
	public static ZeroTask CompletedTask
	{
		get
		{
			Thrower.ThrowIfNotInGameThread();

			return default;
		}
	}

}


