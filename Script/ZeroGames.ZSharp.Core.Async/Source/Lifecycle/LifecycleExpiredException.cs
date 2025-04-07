// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public class LifecycleExpiredException : OperationCanceledException
{

	public LifecycleExpiredException() : base(DEFAULT_MESSAGE){}

	public LifecycleExpiredException(string? message) : base(message ?? DEFAULT_MESSAGE){}

	public LifecycleExpiredException(string? message, Exception? innerException) : base(message ?? DEFAULT_MESSAGE, innerException){}
	
	public LifecycleExpiredException(Lifecycle lifecycle) : this()
	{
		Lifecycle = lifecycle;
	}

	public LifecycleExpiredException(string? message, Lifecycle lifecycle) : this(message)
	{
		Lifecycle = lifecycle;
	}

	public LifecycleExpiredException(string? message, Exception? innerException, Lifecycle lifecycle) : this(message, innerException)
	{
		Lifecycle = lifecycle;
	}
	
	public Lifecycle Lifecycle { get; }

	private const string DEFAULT_MESSAGE = "The operation was canceled - lifecycle expired.";

}