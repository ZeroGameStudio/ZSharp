// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public class LifetimeExpiredException : OperationCanceledException
{

	public LifetimeExpiredException() : base(DEFAULT_MESSAGE){}

	public LifetimeExpiredException(string? message) : base(message ?? DEFAULT_MESSAGE){}

	public LifetimeExpiredException(string? message, Exception? innerException) : base(message ?? DEFAULT_MESSAGE, innerException){}
	
	public LifetimeExpiredException(Lifetime lifetime) : this()
	{
		Lifetime = lifetime;
	}

	public LifetimeExpiredException(string? message, Lifetime lifetime) : this(message)
	{
		Lifetime = lifetime;
	}

	public LifetimeExpiredException(string? message, Exception? innerException, Lifetime lifetime) : this(message, innerException)
	{
		Lifetime = lifetime;
	}
	
	public Lifetime Lifetime { get; }

	private const string DEFAULT_MESSAGE = "The operation was canceled - lifetime expired.";

}