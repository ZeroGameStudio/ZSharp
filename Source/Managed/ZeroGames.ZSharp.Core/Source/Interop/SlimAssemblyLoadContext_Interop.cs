// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class SlimAssemblyLoadContext_Interop
{

    [UnmanagedCallersOnly]
    public static int32 Unload(GCHandle handle) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is SlimAssemblyLoadContext alc)
        {
            alc.Unload();
            return 0;
        }

        return 1;
    }, -1);

    [UnmanagedCallersOnly]
    public static unsafe int32 LoadAssembly(GCHandle handle, uint8* buffer, int32 size, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is SlimAssemblyLoadContext alc)
        {
            Assembly asm = alc.LoadFromStream(new UnmanagedMemoryStream(buffer, size));
            if (DllMainStatics.TryInvokeDllMain(asm, args, out var res) && res is not null)
            {
                return res.Value;
            }

            return 0;
        }
        
        return 1;
    }, -1);

    [UnmanagedCallersOnly]
    public static unsafe int32 CallMethod(GCHandle handle, char* assemblyName, char* typeName, char* methodName, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is SlimAssemblyLoadContext alc)
        {
            Assembly? asm = alc.Assemblies.FirstOrDefault(asm => asm.FullName!.Split(',')[0] == new string(assemblyName));
            if (asm is null)
            {
                return 2;
            }

            string asmName = asm.FullName!.Split(',')[0];
            Type? entryType = asm.GetType($"{asmName}.__DllEntry");
            if (entryType is null)
            {
                return 3;
            }

            MethodInfo? dllMain = entryType.GetMethod("DllMain");
            if (dllMain is null)
            {
                return 4;
            }

            object?[]? parameters = args is not null ? new object?[] { new IntPtr(args) } : null;
            object? res = dllMain.Invoke(null, parameters);

            return res?.GetType() == typeof(int32) ? (int32)res : 0;
        }

        return 1;
    }, -1);

}


