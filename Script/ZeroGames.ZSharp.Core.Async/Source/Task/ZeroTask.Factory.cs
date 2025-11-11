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

	public static ZeroTask FromExpired(Lifetime lifetime) => FromExpired<AsyncVoid>(lifetime);
	public static ZeroTask<T> FromExpired<T>(Lifetime lifetime)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (!lifetime.IsExpired)
		{
			throw new ArgumentOutOfRangeException(nameof(lifetime));
		}
		
		return FromException<T>(new LifetimeExpiredException(lifetime));
	}
	
	public static ZeroTask FromBackend(IZeroTaskBackend backend, Lifetime lifetime = default)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(backend);
	}

	public static ZeroTask<T> FromBackend<T>(IZeroTaskBackend<T> backend, Lifetime lifetime = default)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(backend);
	}

	public static ZeroTask FromCompletionSource(out ZeroTaskCompletionSource source, Lifetime lifetime = default)
	{
		source = ZeroTaskCompletionSource.Create();
		return source.Task;
	}

	public static ZeroTask<T> FromCompletionSource<T>(out ZeroTaskCompletionSource<T> source, Lifetime lifetime = default)
	{
		source = ZeroTaskCompletionSource.Create<T>();
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


