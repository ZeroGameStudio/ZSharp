// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public unsafe struct CommonDllMainArgs
{
    public string[] ToStringArray()
    {
        string[] args = new string[Argc];
        for (int32 i = 0; i < Argc; ++i)
        {
            args[i] = new(Argv[i]);
        }

        return args;
    }
    
    public int32 Argc;
    public char** Argv;
}


