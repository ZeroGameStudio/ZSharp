// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

internal class DllEntry
{

    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct UnmanagedFunction
    {
        public char* TypeName;
        public char* FieldName;
        public void* Function;
    }

    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct Args
    {
        public int32 Argc;
        public UnmanagedFunction* Argv;
    }

    [UnmanagedCallersOnly]
    private static unsafe int32 DllMain(Args* args)
    {
        for (int32 i = 0; i < args->Argc; ++i)
        {
            UnmanagedFunction* function = args->Argv + i;
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
            
            field.SetValue(null, (IntPtr)function->Function);
        }

        string x = $"=========================={UnrealEngine_Interop.SIsInGameThread() > 0}";
        fixed (char* s = x.ToCharArray())
        {
            UnrealEngine_Interop.SLog(3, s);
        }

        return 0;
    }
    
}


