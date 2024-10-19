// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static partial class Assertion
{
	
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Check([DoesNotReturnIf(false)] bool condition) => Check(condition, null);
	
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Check([DoesNotReturnIf(false)] bool condition, string? message)
	{
		
	}

	[Conditional("ASSERTION_CHECK_SLOW")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void CheckSlow([DoesNotReturnIf(false)] bool condition) => CheckSlow(condition, null);
	
	[Conditional("ASSERTION_CHECK_SLOW")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void CheckSlow([DoesNotReturnIf(false)] bool condition, string? message)
	{
		//CallerArgumentExpressionAttribute
	}
	
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void CheckNoReentry()
	{
		
	}

	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void CheckNoRecursion()
	{
		
	}
	
}


