// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public static class UnobservedZeroTaskExceptionPublisher
{

	public static void Publish(Exception exception)
	{
		_unobservedTaskExceptionHandler(exception);
	}

	public static void SetHandler(Action<Exception> handler)
	{
		_unobservedTaskExceptionHandler = handler;
	}
	
	private static Action<Exception> _unobservedTaskExceptionHandler = ex => UnhandledExceptionHelper.Guard(ex, "Unobserved exception detected in ZeroTask.");
	
}


