using ZeroGames.ZSharp.Interop;

namespace ZeroGames.ZSharp;

public static unsafe class Logger
{

    public static void Log(object message)
    {
        string? messageStr = message.ToString();
        if (messageStr is null)
        {
            return;
        }
        
        fixed (char* buffer = messageStr.ToCharArray())
        {
            UE_Interop.GUE_Log(1, buffer);
        }
    }
    
    public static void Warning(object message)
    {
        string? messageStr = message.ToString();
        if (messageStr is null)
        {
            return;
        }
        
        fixed (char* buffer = messageStr.ToCharArray())
        {
            UE_Interop.GUE_Log(2, buffer);
        }
    }
    
    public static void Error(object message)
    {
        string? messageStr = message.ToString();
        if (messageStr is null)
        {
            return;
        }
        
        fixed (char* buffer = messageStr.ToCharArray())
        {
            UE_Interop.GUE_Log(3, buffer);
        }
    }
    
}