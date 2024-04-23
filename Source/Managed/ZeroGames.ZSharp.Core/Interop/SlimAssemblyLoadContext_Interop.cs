// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class SlimAssemblyLoadContext_Interop
{
    
    [UnmanagedCallersOnly]
    public static int32 Unload(GCHandle handle)
    {
        if (handle.Target is SlimAssemblyLoadContext alc)
        {
            alc.Unload();
            return 0;
        }

        return -1;
    }

    [UnmanagedCallersOnly]
    public static unsafe int32 LoadAssembly(GCHandle handle, uint8* buffer, int32 size, void* args)
    {
        if (handle.Target is SlimAssemblyLoadContext alc)
        {
            Assembly asm = alc.LoadFromStream(new UnmanagedMemoryStream(buffer, size));
        
            string asmName = asm.FullName!.Split(',')[0];
            Type? entryType = asm.GetType($"{asmName}.__DllEntry");
            if (entryType is not null)
            {
                MethodInfo? dllMain = entryType.GetMethod("DllMain");
                if (dllMain is not null)
                {
                    object?[]? parameters = args is not null ? new object?[] { new IntPtr(args) } : null;
                    object? res = dllMain.Invoke(null, parameters);
                    if (res?.GetType() == typeof(int32))
                    {
                        return (int32)res;
                    }
                }
            }
            
            return 0;
        }
        
        return -1;
    }
    
    [UnmanagedCallersOnly]
    public static unsafe int32 CallMethod(GCHandle handle, char* assemblyName, char* typeName, char* methodName, void* args)
    {
        if (handle.Target is SlimAssemblyLoadContext alc)
        {
            Assembly? asm = alc.Assemblies.FirstOrDefault(asm => asm.FullName!.Split(',')[0] == new string(assemblyName));
            if (asm is null)
            {
                return 1;
            }
        
            string asmName = asm.FullName!.Split(',')[0];
            Type? entryType = asm.GetType($"{asmName}.__DllEntry");
            if (entryType is null)
            {
                return 2;
            }
            
            MethodInfo? dllMain = entryType.GetMethod("DllMain");
            if (dllMain is null)
            {
                return 3;
            }

            object?[]? parameters = args is not null ? new object?[] { new IntPtr(args) } : null;
            object? res =dllMain.Invoke(null, parameters);

            return res?.GetType() == typeof(int32) ? (int32)res : 0;
        }
        
        return -1;
    }
    
}