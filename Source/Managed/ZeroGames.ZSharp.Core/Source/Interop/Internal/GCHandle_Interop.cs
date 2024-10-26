// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class GCHandle_Interop
{

    [UnmanagedCallersOnly]
    public static int32 Free(GCHandle handle)
    {
        try
        {
            handle.Free();
            return 0;
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return -1;
        }
    }

}


