// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public class ActorInitializationFailedException(AActor actor, string? message = null, Exception? innerException = null) : Exception(string.IsNullOrWhiteSpace(message) ? DEFAULT_MESSAGE : message, innerException)
{

	public ActorInitializationFailedException(AActor actor, Exception? innerException) : this(actor, null, innerException){}

	public AActor Actor { get; } = actor;

	private const string DEFAULT_MESSAGE = "Actor failed to initialize.";

}


