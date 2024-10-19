// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static class LoggerMacros
{
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_LOG(string category, ELogVerbosity verbosity, string message) => InternalLog(category, verbosity, message);

    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_ERROR(string category, string message) => InternalLog(category, ELogVerbosity.Error, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_WARNING(string category, string message) => InternalLog(category, ELogVerbosity.Warning, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_DISPLAY(string category, string message) => InternalLog(category, ELogVerbosity.Display, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_LOG(string category, string message) => InternalLog(category, ELogVerbosity.Log, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_VERBOSE(string category, string message) => InternalLog(category, ELogVerbosity.Verbose, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_VERY_VERBOSE(string category, string message) => InternalLog(category, ELogVerbosity.VeryVerbose, message);

    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CLOG(bool condition, string category, ELogVerbosity verbosity, string message)
    {
        if (condition)
        {
            UE_LOG(category, verbosity, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CERROR(bool condition, string category, string message)
    {
        if (condition)
        {
            UE_ERROR(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CWARNING(bool condition, string category, string message)
    {
        if (condition)
        {
            UE_WARNING(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CDISPLAY(bool condition, string category, string message)
    {
        if (condition)
        {
            UE_DISPLAY(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CLOG(bool condition, string category, string message)
    {
        if (condition)
        {
            UE_LOG(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CVERBOSE(bool condition, string category, string message)
    {
        if (condition)
        {
            UE_VERBOSE(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CVERY_VERBOSE(bool condition, string category, string message)
    {
        if (condition)
        {
            UE_VERY_VERBOSE(category, message);
        }
    }
    
    private static void InternalLog(string category, ELogVerbosity verbosity, string message)
    {
        unsafe
        {
            fixed (char* categoryBuffer = category)
            fixed (char* messageBuffer = message)
            {
                Log_Interop.Log(categoryBuffer, verbosity, messageBuffer);
            }
        }
    }
    
}