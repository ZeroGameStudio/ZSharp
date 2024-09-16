// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public static class Logger
{

    public static void Fatal(params object?[]? objects) => Log(1, objects);
    public static void Error(params object?[]? objects) => Log(2, objects);
    public static void Warning(params object?[]? objects) => Log(3, objects);
    public static void Display(params object?[]? objects) => Log(4, objects);
    public static void Log(params object?[]? objects) => Log(5, objects);
    public static void Verbose(params object?[]? objects) => Log(6, objects);
    public static void VeryVerbose(params object?[]? objects) => Log(7, objects);
    
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
            fixed (char* buffer = sb.ToString().ToCharArray())
            {
                UnrealEngine_Interop.Log(level, buffer);
            }
        }
    }
    
}