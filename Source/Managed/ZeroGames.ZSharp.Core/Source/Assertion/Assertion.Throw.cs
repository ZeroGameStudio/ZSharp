// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static partial class Assertion
{

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Exception NoEntry() => throw _noEntryException;
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static Exception NotImplemented() => throw _notImplementedException;

	private static InvalidOperationException _noEntryException = new();
	private static NotImplementedException _notImplementedException = new();

}


