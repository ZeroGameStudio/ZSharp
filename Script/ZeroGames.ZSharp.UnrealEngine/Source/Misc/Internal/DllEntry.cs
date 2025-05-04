// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

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

    [DllMain]
    private static unsafe void DllMain(Args* args)
    {
        check(MasterAlcCache.Instance == IMasterAssemblyLoadContext.Instance);
        
        for (int32 i = 0; i < args->UnmanagedFunctions.Count; ++i)
        {
            UnmanagedFunction* function = args->UnmanagedFunctions.Functions + i;
            string typeName = new(function->TypeName);
            string fieldName = new(function->FieldName);
            InteropBindingHelper.GetStaticFunctionPointerField(typeName, fieldName).SetValue(null, (IntPtr)function->Address);
        }
        
        int32 offset = 0;
        // StreamableManager interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, IntPtr, int32, void>)&StreamableManager_Interop.Update;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, IntPtr, void>)&StreamableManager_Interop.SignalCompletion;

        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ConjugateHandle, void>)&World_Interop.NotifyWorldInitialized;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ConjugateHandle, void>)&World_Interop.NotifyWorldTearingDown;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, void>)&World_Interop.PreLoadMap;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ConjugateHandle, void>)&World_Interop.PostLoadMap;
        
        UE_LOG(LogZSharpScriptEngine, "===================== Z# Engine Startup =====================");
    }

}


