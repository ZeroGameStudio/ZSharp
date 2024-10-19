// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core;

public static class LogMacros
{
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_LOG(string category, ELogVerbosity verbosity, object message) => InternalLog(category, verbosity, message);

    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_ERROR(string category, object message) => InternalLog(category, ELogVerbosity.Error, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_WARNING(string category, object message) => InternalLog(category, ELogVerbosity.Warning, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_DISPLAY(string category, object message) => InternalLog(category, ELogVerbosity.Display, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_LOG(string category, object message) => InternalLog(category, ELogVerbosity.Log, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_VERBOSE(string category, object message) => InternalLog(category, ELogVerbosity.Verbose, message);
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_VERY_VERBOSE(string category, object message) => InternalLog(category, ELogVerbosity.VeryVerbose, message);

    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CLOG(bool condition, string category, ELogVerbosity verbosity, object message)
    {
        if (condition)
        {
            UE_LOG(category, verbosity, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CERROR(bool condition, string category, object message)
    {
        if (condition)
        {
            UE_ERROR(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CWARNING(bool condition, string category, object message)
    {
        if (condition)
        {
            UE_WARNING(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CDISPLAY(bool condition, string category, object message)
    {
        if (condition)
        {
            UE_DISPLAY(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CLOG(bool condition, string category, object message)
    {
        if (condition)
        {
            UE_LOG(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CVERBOSE(bool condition, string category, object message)
    {
        if (condition)
        {
            UE_VERBOSE(category, message);
        }
    }
    
    [Conditional("UE_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void UE_CVERY_VERBOSE(bool condition, string category, object message)
    {
        if (condition)
        {
            UE_VERY_VERBOSE(category, message);
        }
    }
    
    private static void InternalLog(string category, ELogVerbosity verbosity, object message)
    {
        unsafe
        {
            fixed (char* categoryBuffer = category)
            fixed (char* messageBuffer = message.ToString())
            {
                Log_Interop.Log(categoryBuffer, verbosity, messageBuffer);
            }
        }
    }
    
}