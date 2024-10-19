// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace ZeroGames.ZSharp.Core;

internal static class Logger
{

    [DoesNotReturn]
    public static void Fatal(params object?[]? objects)
    {
        Log(1, objects);
        throw new InvalidOperationException();
    }
    
    public static void Error(params object?[]? objects) => Log(2, objects);
    public static void Warning(params object?[]? objects) => Log(3, objects);
    public static void Display(params object?[]? objects) => Log(4, objects);
    public static void Log(params object?[]? objects) => Log(5, objects);
    public static void Verbose(params object?[]? objects) => Log(6, objects);
    public static void VeryVerbose(params object?[]? objects) => Log(7, objects);
    
    public static void CFatal([DoesNotReturnIf(true)] bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Fatal(objects);
        }
    }
    
    public static void CError(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Error(objects);
        }
    }
    
    public static void CWarning(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Warning(objects);
        }
    }
    
    public static void CDisplay(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Display(objects);
        }
    }
    
    public static void CLog(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Log(objects);
        }
    }
    
    public static void CVerbose(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            Verbose(objects);
        }
    }
    
    public static void CVeryVerbose(bool condition, params object?[]? objects)
    {
        if (condition)
        {
            VeryVerbose(objects);
        }
    }
    
    private static void Log(uint8 level, params object?[]? objects)
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

        unsafe
        {
            fixed (char* buffer = sb.ToString())
            {
                Core_Interop.CoreLog(level, buffer);
            }
        }
    }
    
}