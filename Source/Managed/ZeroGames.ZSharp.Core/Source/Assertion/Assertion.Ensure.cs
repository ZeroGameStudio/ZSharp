// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static partial class Assertion
{
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool Ensure([DoesNotReturnIf(false)] bool condition) => Ensure(condition, null);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool Ensure([DoesNotReturnIf(false)] bool condition, string? message)
	{
#if ASSERTION_CHECK
		
#endif
		
		return condition;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool EnsureAlways([DoesNotReturnIf(false)] bool condition) => EnsureAlways(condition, null);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool EnsureAlways([DoesNotReturnIf(false)] bool condition, string? message)
	{
#if ASSERTION_CHECK_SLOW
		
#endif
		
		return condition;
	}
	
}


