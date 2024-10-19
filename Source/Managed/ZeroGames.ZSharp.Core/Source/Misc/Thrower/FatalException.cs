// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Misc.Thrower;

public class FatalException : Exception
{
	public FatalException(){}
	public FatalException(string? message) : base(message){}
	public FatalException(string? message, Exception? innerException) : base(message, innerException){}
}


