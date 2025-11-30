// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public readonly struct CpuProfilerEventScope : IDisposable
{
    public CpuProfilerEventScope(string name, [CallerFilePath] string file = "", [CallerLineNumber] int32 line = 0)
    {
        if (string.IsNullOrWhiteSpace(name))
        {
            return;
        }
        
        unsafe
        {
            fixed (char* nameBuffer = name)
            fixed (char* fileBuffer = file)
            {
                _unmanaged = Profiling_Interop.BeginCpuProfilerEvent(nameBuffer, fileBuffer, line);
            }
        }
    }

    public void Dispose()
    {
        if (_unmanaged == IntPtr.Zero)
        {
            return;
        }
        
        unsafe
        {
            Profiling_Interop.EndCpuProfilerEvent(_unmanaged);
        }
    }

    private readonly IntPtr _unmanaged;
}


