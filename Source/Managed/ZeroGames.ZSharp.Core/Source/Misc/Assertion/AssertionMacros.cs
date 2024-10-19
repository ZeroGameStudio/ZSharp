// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public static class AssertionMacros
{
	
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void check
	(
		[DoesNotReturnIf(false)] bool condition,
		string? message = default,
		[CallerArgumentExpression(nameof(condition))] string? expr = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
		CallerInfoHelper.Inject(ref column);
		Assertion.Check(condition, message, expr, file, line, column);
	}

	[Conditional("ASSERTION_CHECK_SLOW")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void checkSlow
	(
		[DoesNotReturnIf(false)] bool condition,
		string? message = default,
		[CallerArgumentExpression(nameof(condition))] string? expr = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
		CallerInfoHelper.Inject(ref column);
		Assertion.CheckSlow(condition, message, expr, file, line, column);
	}
	
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void checkNoEntry
	(
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
		CallerInfoHelper.Inject(ref column);
		Assertion.CheckNoEntry(file, line, column);
	}

	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void checkNoReentry
	(
		AssemblyLoadContext? context = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
		CallerInfoHelper.Inject(ref context, ref column);
		Assertion.CheckNoReentry(context, file, line, column);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static IDisposable checkNoRecursion
	(
		AssemblyLoadContext? context = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK
		CallerInfoHelper.Inject(ref context, ref column);
		return Assertion.CheckNoRecursion(context, file, line, column);
#else
		return null;
#endif
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void verify
	(
		[DoesNotReturnIf(false)] bool condition,
		string? message = default,
		[CallerArgumentExpression(nameof(condition))] string? expr = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK
		CallerInfoHelper.Inject(ref column);
		Assertion.Verify(condition, message, expr, file, line, column);
#endif
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void verifySlow
	(
		[DoesNotReturnIf(false)] bool condition,
		string? message = default,
		[CallerArgumentExpression(nameof(condition))] string? expr = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK_SLOW
		CallerInfoHelper.Inject(ref column);
		Assertion.VerifySlow(condition, message, expr, file, line, column);
#endif
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void ensure
	(
		[DoesNotReturnIf(false)] bool condition,
		string? message = default,
		AssemblyLoadContext? context = default,
		[CallerArgumentExpression(nameof(condition))] string? expr = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK
		CallerInfoHelper.Inject(ref context, ref column);
		Assertion.Ensure(condition, message, context, expr, file, line, column);
#endif
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void ensureAlways
	(
		[DoesNotReturnIf(false)] bool condition,
		string? message = default,
		[CallerArgumentExpression(nameof(condition))] string? expr = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK_SLOW
		CallerInfoHelper.Inject(ref column);
		Assertion.EnsureAlways(condition, message, expr, file, line, column);
#endif
	}
	
}


