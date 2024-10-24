﻿// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public static class Uncaught
{

    public static T ErrorIfUncaught<T>(Func<T> func, T fallback)
    {
        try
        {
            return func();
        }
        catch (Exception ex)
        {
            Debugger.Break();
            UE_ERROR(LogZSharpScriptEngine, $"Unhandled Exception Detected.\n{ex}");
            return fallback;
        }
    }

    public static void ErrorIfUncaught(Action action)
    {
        try
        {
            action();
        }
        catch (Exception ex)
        {
            Debugger.Break();
            UE_ERROR(LogZSharpScriptEngine, $"Unhandled Exception Detected.\n{ex}");
        }
    }
    
    public static T FatalIfUncaught<T>(Func<T> func, T fallback)
    {
        try
        {
            return func();
        }
        catch (Exception ex)
        {
            Debugger.Break();
            UE_ERROR(LogZSharpScriptEngine, $"Unhandled Fatal Exception!!!\n{ex}");
            return fallback;
        }
    }

    public static void FatalIfUncaught(Action action)
    {
        try
        {
            action();
        }
        catch (Exception ex)
        {
            Debugger.Break();
            UE_ERROR(LogZSharpScriptEngine, $"Unhandled Fatal Exception!!!\n{ex}");
        }
    }
    
}