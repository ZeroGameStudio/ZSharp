// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
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
		if (!condition)
		{
			Fail(message, expr, file, line, column, false);
		}
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
		check(condition, message, expr, file, line, column);
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
		check(false, "Enclosing block should never be called!", null, file, line, column);
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
		if (!_reentryCache.Add(new(context, file ?? string.Empty, line, column)))
		{
			check(false, "Enclosing block was called more than once!", null, file, line, column);
		}
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static IDisposable? checkNoRecursion
	(
		AssemblyLoadContext? context = default,
		[CallerFilePath] string? file = default,
		[CallerLineNumber] int32 line = default,
		[CallerColumnNumber] int32 column = default
	)
	{
#if ASSERTION_CHECK
		CallerInfoHelper.Inject(ref context, ref column);
		Coordinate coord = new(context, file ?? string.Empty, line, column);
		if (_recursionCache.Add(coord))
		{
			return new RecursionScope(coord);
		}
		
		check(false, "Enclosing block was entered recursively!", null, file, line, column);
#endif
		
		return null;
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
		check(condition, message, expr, file, line, column);
#else
		if (!condition)
		{
			throw new AssertionFailedException();
		}
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
		verify(condition, message, expr, file, line, column);
#else
		if (!condition)
		{
			throw new AssertionFailedException();
		}
#endif
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool ensure
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
		if (_ensureCache.Add(new(context, file ?? string.Empty, line, column)))
		{
			ensureAlways(condition, message, expr, file, line, column);
		}
#endif

		return condition;
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool ensureAlways
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
		
		return condition;
	}
	
	[DoesNotReturn]
	private static void Fail(string? message, string? expr, string? file, int32 line, int32 column, bool forceNoFatal)
	{
		string finalMessage = $"Assertion [{expr}] failed: {message} at file {file} line {line} column {column}.";
		if (Debugger.IsAttached || forceNoFatal)
		{
			CoreLog.Error(finalMessage);
			Debugger.Break();
		}
		else
		{
			Thrower.Fatal(finalMessage);
		}
		
		throw new AssertionFailedException(finalMessage);
	}

	private readonly struct RecursionScope(Coordinate coord) : IDisposable
	{
		public void Dispose() => Thrower.FatalIf(!_recursionCache.Remove(_coord));
		private readonly Coordinate _coord = coord;
	}

	private readonly record struct Coordinate
	{
		public Coordinate(AssemblyLoadContext context, string file, int32 line, int32 column)
		{
			Context = context;
			File = file;
			Line = line;
			Column = column;
			
			if (_relevantContexts.Add(context))
			{
				context.Unloading += static alc =>
				{
					if (_relevantContexts.Remove(alc))
					{
						ClearCache(_reentryCache, alc);
						ClearCache(_recursionCache, alc);
						ClearCache(_ensureCache, alc);
					}
				};
			}
		}
		
		public AssemblyLoadContext Context { get; }
		public string File { get; }
		public int32 Line { get; }
		public int32 Column { get; }

		private static void ClearCache(HashSet<Coordinate> cache, AssemblyLoadContext context)
		{
			foreach (var coord in cache)
			{
				if (coord.Context == context)
				{
					cache.Remove(coord);
				}
			}
		}
		
		private static readonly HashSet<AssemblyLoadContext> _relevantContexts = new();
	}

	private static readonly HashSet<Coordinate> _reentryCache = new();
	private static readonly HashSet<Coordinate> _recursionCache = new();
	private static readonly HashSet<Coordinate> _ensureCache = new();
	
}


