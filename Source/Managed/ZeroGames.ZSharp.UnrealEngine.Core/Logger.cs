// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public static class Logger
{

    private static void Log(uint8 level, params object?[]? objects)
    {
        StringBuilder sb = new();
        if (objects is not null)
        {
            bool bFirst = true;
            foreach (var obj in objects)
            {
                if (bFirst)
                {
                    bFirst = false;
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
                UnrealEngine_Interop.SLog(level, buffer);
            }
        }
    }

    public static void Error(params object?[]? objects) => Log(2, objects);
    public static void Warning(params object?[]? objects) => Log(3, objects);
    public static void Display(params object?[]? objects) => Log(4, objects);
    public static void Log(params object?[]? objects) => Log(5, objects);
    public static void Verbose(params object?[]? objects) => Log(6, objects);
    public static void VeryVerbose(params object?[]? objects) => Log(7, objects);
    
}