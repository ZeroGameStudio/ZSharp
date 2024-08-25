// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

[assembly: AssemblyResolver(typeof(ZeroGames.ZSharp.AssemblyResolver.AssemblyResolver))]

namespace ZeroGames.ZSharp.AssemblyResolver;

internal static class DllEntry
{

    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct Args
    {
        public char* ProjectDir;
    }

    [UnmanagedCallersOnly]
    private static unsafe void DllMain(Args* args)
    {
        GlobalRegistry.ProjectDir = new(args->ProjectDir);
    }
    
}


