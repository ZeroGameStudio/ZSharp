﻿// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

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


