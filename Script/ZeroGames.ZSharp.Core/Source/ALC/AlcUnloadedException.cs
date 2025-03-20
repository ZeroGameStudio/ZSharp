// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public class AlcUnloadedException : InvalidOperationException
{
	public AlcUnloadedException(){}
	public AlcUnloadedException(string? message) : base(message){}
	public AlcUnloadedException(string? message, Exception? innerException) : base(message, innerException){}
}


