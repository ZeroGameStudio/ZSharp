// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public partial struct ZeroTask
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
		
		throw new NotImplementedException();
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
	
	public static ZeroTask FromBackend(IZeroTaskBackend backend)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(backend);
	}

	public static ZeroTask<T> FromBackend<T>(IZeroTaskBackend<T> backend)
	{
		Thrower.ThrowIfNotInGameThread();

		return new(backend);
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


