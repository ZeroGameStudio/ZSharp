// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public class AssertionFailedException : Exception
{
	public AssertionFailedException(){}
	public AssertionFailedException(string? message) : base(message){}
	public AssertionFailedException(string? message, Exception? innerException) : base(message, innerException){}
}


