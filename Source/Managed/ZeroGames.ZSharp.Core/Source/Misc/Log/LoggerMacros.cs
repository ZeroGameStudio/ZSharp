// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static class LoggerMacros
{

	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_ERROR(params object?[]? objects) => Logger.Error(objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_WARNING(params object?[]? objects) => Logger.Warning(objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_DISPLAY(params object?[]? objects) => Logger.Display(objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_LOG(params object?[]? objects) => Logger.Log(objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_VERBOSE(params object?[]? objects) => Logger.Verbose(objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_VERY_VERBOSE(params object?[]? objects) => Logger.VeryVerbose(objects);
	
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_CERROR(bool condition, params object?[]? objects) => Logger.CError(condition, objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_CWARNING(bool condition, params object?[]? objects) => Logger.CWarning(condition, objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_CDISPLAY(bool condition, params object?[]? objects) => Logger.CDisplay(condition, objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_CLOG(bool condition, params object?[]? objects) => Logger.CLog(condition, objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_CVERBOSE(bool condition, params object?[]? objects) => Logger.CVerbose(condition, objects);
    
	[Conditional("UE_LOG")]
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void UE_CVERY_VERBOSE(bool condition, params object?[]? objects) => Logger.CVeryVerbose(condition, objects);
	
}


