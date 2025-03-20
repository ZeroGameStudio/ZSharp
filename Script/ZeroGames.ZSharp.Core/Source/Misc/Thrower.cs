// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static class Thrower
{

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	[DoesNotReturn]
	public static Exception Fatal(string? message = default, Exception? innerException = default)
	{
		if (string.IsNullOrWhiteSpace(message) && innerException == default)
		{
			throw _defaultFatalException;
		}
		
		throw new FatalException(message, innerException);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void FatalIf([DoesNotReturnIf(true)] bool condition, string? message = default, Exception? innerException = default)
	{
		if (condition)
		{
			Fatal(message, innerException);
		}
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void ThrowIfNotInGameThread(string? message = "Operation occurs in non-game thread.", Exception? innerException = default)
	{
		if (!GameThreadScheduler.IsInGameThread)
		{
			throw new InvalidOperationException(message);
		}
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	[DoesNotReturn]
	public static Exception NoEntry() => throw _noEntryException;
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	[DoesNotReturn]
	public static Exception NotImplemented() => throw _notImplementedException;

	private static readonly FatalException _defaultFatalException = new();
	private static readonly InvalidOperationException _noEntryException = new();
	private static readonly NotImplementedException _notImplementedException = new();
	
}


