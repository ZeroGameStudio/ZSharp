// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static class AssertionMacros
{
	
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void check([DoesNotReturnIf(false)] bool condition) => Assertion.Check(condition);

	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void check([DoesNotReturnIf(false)] bool condition, string? message) => Assertion.Check(condition, message);

	[Conditional("ASSERTION_CHECK_SLOW")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void checkSlow([DoesNotReturnIf(false)] bool condition) => Assertion.CheckSlow(condition);
	
	[Conditional("ASSERTION_CHECK_SLOW")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void checkSlow([DoesNotReturnIf(false)] bool condition, string? message) => Assertion.CheckSlow(condition, message);
	
	// The following macros depend on call stack, so we don't forward to Assertion but give the same implementation.
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void checkNoReentry()
	{
		
	}

	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void checkNoRecursion()
	{
		
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void verify([DoesNotReturnIf(false)] bool condition) => Assertion.Verify(condition);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void verify([DoesNotReturnIf(false)] bool condition, string? message) => Assertion.Verify(condition, message);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void verifySlow([DoesNotReturnIf(false)] bool condition) => Assertion.VerifySlow(condition);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void verifySlow([DoesNotReturnIf(false)] bool condition, string? message) => Assertion.VerifySlow(condition, message);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool ensure([DoesNotReturnIf(false)] bool condition) => Assertion.Ensure(condition);

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool ensure([DoesNotReturnIf(false)] bool condition, string? message) => Assertion.Ensure(condition, message);

	// The following macros depend on call stack, so we don't forward to Assertion but give the same implementation.
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool ensureAlways([DoesNotReturnIf(false)] bool condition)
	{
		return condition;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool ensureAlways([DoesNotReturnIf(false)] bool condition, string? message)
	{
		return condition;
	}
	
}


