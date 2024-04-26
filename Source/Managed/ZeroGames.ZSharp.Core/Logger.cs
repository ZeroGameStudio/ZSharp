// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.Core;

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

    public static void Log(params object?[]? objects) => Log(1, objects);
    
    public static void Warning(params object?[]? objects) => Log(2, objects);
    
    public static void Error(params object?[]? objects) => Log(3, objects);
    
}