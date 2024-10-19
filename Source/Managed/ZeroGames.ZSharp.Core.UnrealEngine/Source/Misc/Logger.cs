// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Text;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public static class Logger
{

    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void Error(params object?[]? objects) => Log(2, objects);
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void Warning(params object?[]? objects) => Log(3, objects);
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void Display(params object?[]? objects) => Log(4, objects);
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void Log(params object?[]? objects) => Log(5, objects);
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void Verbose(params object?[]? objects) => Log(6, objects);
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void VeryVerbose(params object?[]? objects) => Log(7, objects);

    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void CError(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Error(objects);
        }
    }
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void CWarning(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Warning(objects);
        }
    }
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void CDisplay(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Display(objects);
        }
    }
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void CLog(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Log(objects);
        }
    }
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void CVerbose(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Verbose(objects);
        }
    }
    
    [Conditional("SCRIPT_LOG")]
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void CVeryVerbose(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            VeryVerbose(objects);
        }
    }
    
    private static void Log(uint8 level, params object?[]? objects)
    {
        unsafe
        {
            fixed (char* buffer = GetMessage(objects))
            {
                UnrealEngine_Interop.Log(level, buffer);
            }
        }
    }

    private static string GetMessage(object?[]? objects)
    {
        StringBuilder sb = new();
        if (objects is not null)
        {
            bool isFirst = true;
            foreach (var obj in objects)
            {
                if (isFirst)
                {
                    isFirst = false;
                }
                else
                {
                    sb.Append("\t");
                }
                
                sb.Append(obj);
            }
        }

        return sb.ToString();
    }
    
}