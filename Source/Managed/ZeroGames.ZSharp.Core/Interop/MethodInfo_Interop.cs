using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class MethodInfo_Interop
{

    [UnmanagedCallersOnly]
    public static int32 GetName(GCHandle handle, IntPtr outNameAddr)
    {
        if (handle.Target is MethodInfo method)
        {
            InteropString interopStr = new(outNameAddr);
            string fullName = method.Name;
            string name = fullName.Split(',')[0];
            interopStr.Data = name;

            return 0;
        }

        return 1;
    }

    [UnmanagedCallersOnly]
    public static unsafe int32 Invoke(GCHandle handle, ZCallBuffer* buffer)
    {
        if (handle.Target is MethodInfo method)
        {
            object? obj = null;
            List<object?> parameters = new();
            
            int32 pos = 0;
            if (!method.IsStatic)
            {
                Type thisType = method.DeclaringType!;
                obj = buffer->Slots[pos++].Conjugate.ToGCHandle().Target;
                if (obj is null || !obj.GetType().IsAssignableTo(thisType))
                {
                    return -1;
                }
            }

            foreach (var parameterInfo in method.GetParameters())
            {
                object? parameter = null;
                Type parameterType = parameterInfo.ParameterType;
                if (parameterType == typeof(uint8))
                {
                    parameter = buffer->Slots[pos++].UInt8;
                }
                else if (parameterType == typeof(uint16))
                {
                    parameter = buffer->Slots[pos++].UInt16;
                }
                else if (parameterType == typeof(uint32))
                {
                    parameter = buffer->Slots[pos++].UInt32;
                }
                else if (parameterType == typeof(uint64))
                {
                    parameter = buffer->Slots[pos++].UInt64;
                }
                else if (parameterType == typeof(int8))
                {
                    parameter = buffer->Slots[pos++].Int8;
                }
                else if (parameterType == typeof(int16))
                {
                    parameter = buffer->Slots[pos++].Int16;
                }
                else if (parameterType == typeof(int32))
                {
                    parameter = buffer->Slots[pos++].Int32;
                }
                else if (parameterType == typeof(int64))
                {
                    parameter = buffer->Slots[pos++].Int64;
                }
                else if (parameterType == typeof(float))
                {
                    parameter = buffer->Slots[pos++].Float;
                }
                else if (parameterType == typeof(double))
                {
                    parameter = buffer->Slots[pos++].Double;
                }
                else if (parameterType == typeof(bool))
                {
                    parameter = buffer->Slots[pos++].Bool != 0;
                }
                else if (parameterType.IsAssignableTo(typeof(IConjugate)))
                {
                    parameter = buffer->Slots[pos++].Conjugate.ToGCHandle().Target;
                    if (parameter is not null && !parameter.GetType().IsAssignableTo(parameterType))
                    {
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }
                
                parameters.Add(parameter);
            }
            
            object? returnValue = method.Invoke(obj, parameters.ToArray());
            if (method.ReturnType != typeof(void))
            { 
                Type returnType = method.ReturnType;
                if (returnType == typeof(uint8))
                {
                    buffer->Slots[pos++].UInt8 = (uint8)returnValue!;
                }
                else if (returnType == typeof(uint16))
                {
                    buffer->Slots[pos++].UInt16 = (uint16)returnValue!;
                }
                else if (returnType == typeof(uint32))
                {
                    buffer->Slots[pos++].UInt32 = (uint32)returnValue!;
                }
                else if (returnType == typeof(uint64))
                {
                    buffer->Slots[pos++].UInt64 = (uint64)returnValue!;
                }
                else if (returnType == typeof(int8))
                {
                    buffer->Slots[pos++].Int8 = (int8)returnValue!;
                }
                else if (returnType == typeof(int16))
                {
                    buffer->Slots[pos++].Int16 = (int16)returnValue!;
                }
                else if (returnType == typeof(int32))
                {
                    buffer->Slots[pos++].Int32 = (int32)returnValue!;
                }
                else if (returnType == typeof(int64))
                {
                    buffer->Slots[pos++].Int64 = (int64)returnValue!;
                }
                else if (returnType == typeof(float))
                {
                    buffer->Slots[pos++].Float = (float)returnValue!;
                }
                else if (returnType == typeof(double))
                {
                    buffer->Slots[pos++].Double = (double)returnValue!;
                }
                else if (returnType == typeof(bool))
                {
                    buffer->Slots[pos++].Bool = (uint8)((bool)returnValue! ? 1 : 0);
                }
                else if (returnType.IsAssignableTo(typeof(IConjugate)))
                {
                    if (returnValue is not null && !returnValue.GetType().IsAssignableTo(returnType))
                    {
                        return -1;
                    }
                    buffer->Slots[pos++].Conjugate = ConjugateHandle.FromConjugate((IConjugate?)returnValue);
                }
                else
                {
                    return -1;
                }
            }
            
            return 0;
        }

        return -1;
    }
    
}


