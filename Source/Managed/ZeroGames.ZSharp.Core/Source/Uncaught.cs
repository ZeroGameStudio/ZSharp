﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal static class Uncaught
{

    public static T ErrorIfUncaught<T>(Func<T> func, T fallback)
    {
        try
        {
            return func();
        }
        catch (Exception ex)
        {
            Logger.Error($"Unhandled Exception Detected.\n{ex}");
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
            Logger.Error($"Unhandled Exception Detected.\n{ex}");
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
            Logger.Fatal($"Unhandled Fatal Exception!!!\n{ex}");
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
            Logger.Fatal($"Unhandled Fatal Exception!!!\n{ex}");
        }
    }
    
}