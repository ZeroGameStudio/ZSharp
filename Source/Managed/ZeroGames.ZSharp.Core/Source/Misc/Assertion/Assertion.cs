// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public static class Assertion
{

	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Check
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
		if (!condition)
		{
			Fail(expr, message, file, line, column, false);
		}
	}

	[Conditional("ASSERTION_CHECK_SLOW")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void CheckSlow
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
		Check(condition, message, expr, file, line, column);
	}
	
	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void CheckNoEntry
	(
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
		CallerInfoHelper.Inject(ref column);
		Check(false, "Enclosing block should never be called!", null, file, line, column);
	}

	[Conditional("ASSERTION_CHECK")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void CheckNoReentry
	(
		GCHandle? context = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
		CallerInfoHelper.Inject(ref context, ref column);
		if (!_reentryCache.Add(new(context!.Value, file ?? string.Empty, line, column)))
		{
			Check(false, "Enclosing block was called more than once!", null, file, line, column);
		}
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static IDisposable CheckNoRecursion
	(
		GCHandle? context = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK
		CallerInfoHelper.Inject(ref context, ref column);
		Coordinate coord = new(context!.Value, file ?? string.Empty, line, column);
		if (_recursionCache.Add(coord))
		{
			return new RecursionScope(coord);
		}
		
		Check(false, "Enclosing block was entered recursively!", null, file, line, column);
#endif
		
		return null;
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Verify
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
		Check(condition, message, expr, file, line, column);
#endif
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void VerifySlow
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
		Verify(condition, message, expr, file, line, column);
#endif
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Ensure
	(
		[DoesNotReturnIf(false)] bool condition,
		string? message = default,
		GCHandle? context = default,
		[CallerArgumentExpression(nameof(condition))] string? expr = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK
		CallerInfoHelper.Inject(ref context, ref column);
		if (_ensureCache.Add(new(context!.Value, file ?? string.Empty, line, column)))
		{
			EnsureAlways(condition, message, expr, file, line, column);
		}
#endif
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void EnsureAlways
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
		if (!condition)
		{
			Fail(message, expr, file, line, column, true);
		}
#endif
	}
	
	private static void Fail(string? message, string? expr, string? file, int32 line, int32 column, bool forceNoFatal)
	{
		string finalMessage = $"Assertion [{expr}] failed: {message} at file {file} line {line} column {column}.";
		if (Debugger.IsAttached || forceNoFatal)
		{
			Logger.Error(finalMessage);
			Debugger.Break();
		}
		else
		{
			Thrower.Fatal(finalMessage);
		}
	}

	private readonly struct RecursionScope(Coordinate coord) : IDisposable
	{
		public void Dispose() => Thrower.FatalIf(!_recursionCache.Remove(coord));
	}

	private readonly record struct Coordinate(GCHandle Context, string File, int32 Line, int32 Column);

	private static HashSet<Coordinate> _reentryCache = new();
	private static HashSet<Coordinate> _recursionCache = new();
	private static HashSet<Coordinate> _ensureCache = new();
	
}


