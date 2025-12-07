// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using ZeroGames.Extensions.Math;
using ZeroGames.ZSharp.Core.UnrealEngine.Specifier;
using ZeroGames.ZSharp.Emit.Specifier;

[assembly: DllEntry(typeof(DllEntry))]

namespace ZeroGames.ZSharp.UnrealEngine;

internal static class DllEntry
{
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct UnmanagedFunction
    {
        public char* TypeName;
        public char* FieldName;
        public void* Address;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct UnmanagedFunctions
    {
        public int32 Count;
        public UnmanagedFunction* Functions;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct Args
    {
        public UnmanagedFunctions UnmanagedFunctions;
        public void*** ManagedFunctions;
    }

    [Conditional("ASSERTION_CHECK")]
    private static unsafe void CheckMemoryLayout()
    {
        check(sizeof(Vector) == 24);
        check(Marshal.OffsetOf<Vector>(nameof(Vector.X)) == 0);
        check(Marshal.OffsetOf<Vector>(nameof(Vector.Y)) == 8);
        check(Marshal.OffsetOf<Vector>(nameof(Vector.Z)) == 16);
        
        check(sizeof(Rotator) == 24);
        check(Marshal.OffsetOf<Rotator>(nameof(Rotator.Pitch)) == 0);
        check(Marshal.OffsetOf<Rotator>(nameof(Rotator.Yaw)) == 8);
        check(Marshal.OffsetOf<Rotator>(nameof(Rotator.Roll)) == 16);
        
        check(sizeof(Quaternion) == 32);
        check(Marshal.OffsetOf<Quaternion>(nameof(Quaternion.X)) == 0);
        check(Marshal.OffsetOf<Quaternion>(nameof(Quaternion.Y)) == 8);
        check(Marshal.OffsetOf<Quaternion>(nameof(Quaternion.Z)) == 16);
        check(Marshal.OffsetOf<Quaternion>(nameof(Quaternion.W)) == 24);
        
        check(sizeof(Transform) == 96);
        check(Marshal.OffsetOf<Transform>(nameof(Transform.Rotation)) == 0);
        check(Marshal.OffsetOf<Transform>(nameof(Transform.Translation)) == 32);
        check(Marshal.OffsetOf<Transform>(nameof(Transform.Scale)) == 64);
    }

    [DllMain]
    private static unsafe void DllMain(Args* args)
    {
        check(MasterAlcCache.Instance == IMasterAssemblyLoadContext.Instance);

        CheckMemoryLayout();
        
        for (int32 i = 0; i < args->UnmanagedFunctions.Count; ++i)
        {
            UnmanagedFunction* function = args->UnmanagedFunctions.Functions + i;
            string typeName = new(function->TypeName);
            string fieldName = new(function->FieldName);
            InteropBindingHelper.GetStaticFunctionPointerField(typeName, fieldName).SetValue(null, (IntPtr)function->Address);
        }
        
        int32 offset = 0;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<void>)&Engine_Interop.PostEngineInit;
        
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, IntPtr, int32, void>)&StreamableManager_Interop.Update;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, IntPtr, void>)&StreamableManager_Interop.SignalCompletion;

        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ConjugateHandle, void>)&World_Interop.NotifyWorldInitialized;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ConjugateHandle, void>)&World_Interop.NotifyWorldTearingDown;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, void>)&World_Interop.PreLoadMap;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ConjugateHandle, void>)&World_Interop.PostLoadMap;
        
        IMasterAssemblyLoadContext.Instance.OnAssemblyLoaded += static assembly =>
        {
            foreach (var type in assembly.GetTypes())
            {
                if (type is { IsClass: false, IsInterface: false })
                {
                    continue;
                }

                if (type.GetCustomAttribute<UStructAttribute>() is { } attr)
                {
                    // Try to cache StaticStruct.
                    _ = type.GetProperty(nameof(IStaticStruct.StaticStruct), BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly)!.GetValue(null);
                    
                    // ALC has update conjugate registry so we can use conjugate here.
                    string unrealFieldPath = type.GetCustomAttribute<UnrealFieldPathAttribute>()!.Path;
                    
                    var constructThunk = (void*)(IntPtr)type.GetProperty("__PRIVATE_ConstructThunkPtr", BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.DeclaredOnly)!.GetValue(null)!;
                    var destructThunk = (void*)(IntPtr)type.GetProperty("__PRIVATE_DestructThunkPtr", BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.DeclaredOnly)!.GetValue(null)!;

                    fixed (char* unrealFieldPathBuffer = unrealFieldPath)
                    {
                        UnrealFieldEmitter_Interop.InternalReloadStructOpsFakeVTable(unrealFieldPathBuffer, constructThunk, destructThunk);
                    }
                }
            }
        };
        
        UE_LOG(LogZSharpScriptEngine, "===================== Z# Engine Startup =====================");
    }

}


