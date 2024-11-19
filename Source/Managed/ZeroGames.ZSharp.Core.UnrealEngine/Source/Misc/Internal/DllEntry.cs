// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal class DllEntry
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
    private unsafe struct UnmanagedProperties
    {
        public void* IsInGameThreadFuncPtr;
        public uint8* GIsServerPtr;
        public uint8* GIsClientPtr;
        public uint8* GIsEditorPtr;
        public uint64* GFrameCounterPtr;
        public void* GConfig;
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct Args
    {
        public UnmanagedFunctions UnmanagedFunctions;
        public unsafe void*** ManagedFunctions;
        public UnmanagedProperties UnmanagedProperties;
    }

    [UnmanagedCallersOnly]
    private static unsafe void DllMain(Args* args)
    {
        for (int32 i = 0; i < args->UnmanagedFunctions.Count; ++i)
        {
            UnmanagedFunction* function = args->UnmanagedFunctions.Functions + i;
            string typeName = new(function->TypeName);
            string fieldName = new(function->FieldName);
            InteropBindingHelper.GetStaticFunctionPointerField(typeName, fieldName).SetValue(null, (IntPtr)function->Address);
        }
        
        int32 offset = 0;
        // Console interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, IntPtr, void>)&Console_Interop.HandleExecuteCommand;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, void>)&Console_Interop.HandleVariableChanged;
        
        CoreGlobals_Interop.IsInGameThreadFuncPtr = (delegate* unmanaged<uint8>)args->UnmanagedProperties.IsInGameThreadFuncPtr;
        CoreGlobals_Interop.GIsServerPtr = args->UnmanagedProperties.GIsServerPtr;
        CoreGlobals_Interop.GIsClientPtr = args->UnmanagedProperties.GIsClientPtr;
        CoreGlobals_Interop.GIsEditorPtr = args->UnmanagedProperties.GIsEditorPtr;
        CoreGlobals_Interop.GFrameCounterPtr = args->UnmanagedProperties.GFrameCounterPtr;
        GConfig = new Config((IntPtr)args->UnmanagedProperties.GConfig);
    }

}


