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
    private static unsafe void DllMain(Args* args) => Uncaught.FatalIfUncaught(() =>
    {
        for (int32 i = 0; i < args->UnmanagedFunctions.Count; ++i)
        {
            UnmanagedFunction* function = args->UnmanagedFunctions.Functions + i;
            string typeName = new(function->TypeName);
            Type? type = Assembly.GetExecutingAssembly().GetType(typeName);
            if (type is null)
            {
                throw new Exception($"Binding failed, type not found. {typeName}");
            }

            string fieldName = new(function->FieldName);
            FieldInfo? field = type.GetField(fieldName);
            if (field is null)
            {
                throw new Exception($"Binding failed, field not found. {typeName}.{fieldName}");
            }

            if (!field.IsStatic)
            {
                throw new Exception($"Binding failed, field is not static. {typeName}.{fieldName}");
            }

            if (!field.FieldType.IsUnmanagedFunctionPointer)
            {
                throw new Exception($"Binding failed, field is not unmanaged function pointer. {typeName}.{fieldName}");
            }
            
            field.SetValue(null, (IntPtr)function->Address);
        }
    });

}


