// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static partial class Assertion
{
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Verify([DoesNotReturnIf(false)] bool condition) => Verify(condition, null);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Verify([DoesNotReturnIf(false)] bool condition, string? message)
	{
#if ASSERTION_CHECK
		
#endif
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void VerifySlow([DoesNotReturnIf(false)] bool condition) => VerifySlow(condition, null);
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void VerifySlow([DoesNotReturnIf(false)] bool condition, string? message)
	{
#if ASSERTION_CHECK_SLOW
		
#endif
	}
	
}


