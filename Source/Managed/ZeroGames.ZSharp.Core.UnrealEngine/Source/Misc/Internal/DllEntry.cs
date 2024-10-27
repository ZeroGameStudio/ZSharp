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
    private struct Args
    {
        public UnmanagedFunctions UnmanagedFunctions;
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
    }

}


