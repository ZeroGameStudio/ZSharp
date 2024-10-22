// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public class LifecycleExpiredException : Exception
{

	public LifecycleExpiredException(){}

	public LifecycleExpiredException(string? message) : base(message){}

	public LifecycleExpiredException(string? message, Exception? innerException) : base(message, innerException){}
	
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

}