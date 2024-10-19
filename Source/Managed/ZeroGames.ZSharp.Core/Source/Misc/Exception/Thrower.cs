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
			throw _defaultFatalException ??= new();
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
	[DoesNotReturn]
	public static Exception NoEntry() => throw (_noEntryException ??= new());
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	[DoesNotReturn]
	public static Exception NotImplemented() => throw (_notImplementedException ??= new());

	private static FatalException? _defaultFatalException;
	private static InvalidOperationException? _noEntryException;
	private static NotImplementedException? _notImplementedException;
	
}


