// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static class Thrower
{

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	[DoesNotReturn]
	public static Exception Fatal() => Fatal(null);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	[DoesNotReturn]
	public static Exception Fatal(string? message)
	{
		if (string.IsNullOrWhiteSpace(message))
		{
			throw _defaultFatalException ??= new();
		}
		
		throw new FatalException(message);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void FatalIf([DoesNotReturnIf(true)] bool condition) => FatalIf(condition, null);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void FatalIf([DoesNotReturnIf(true)] bool condition, string? message)
	{
		if (condition)
		{
			Fatal(message);
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


