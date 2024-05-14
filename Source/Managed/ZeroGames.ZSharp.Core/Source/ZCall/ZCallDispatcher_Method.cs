// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal class ZCallDispatcher_Method : IZCallDispatcher
{
	
	public unsafe int32 Dispatch(ZCallBuffer* buffer)
	{
        object? obj = null;
        List<object?> parameters = new();

        int32 pos = 0;
        if (!Method.IsStatic)
        {
            Type thisType = Method.DeclaringType!;
            obj = (*buffer)[pos++].ReadConjugate<IConjugate>();
            if (obj is null || !obj.GetType().IsAssignableTo(thisType))
            {
                return 1;
            }
        }

        foreach (var parameterInfo in Method.GetParameters())
        {
            object? parameter = null;
            Type parameterType = parameterInfo.ParameterType;
            if (parameterType == typeof(uint8))
            {
                parameter = (*buffer)[pos++].UInt8;
            }
            else if (parameterType == typeof(uint16))
            {
                parameter = (*buffer)[pos++].UInt16;
            }
            else if (parameterType == typeof(uint32))
            {
                parameter = (*buffer)[pos++].UInt32;
            }
            else if (parameterType == typeof(uint64))
            {
                parameter = (*buffer)[pos++].UInt64;
            }
            else if (parameterType == typeof(int8))
            {
                parameter = (*buffer)[pos++].Int8;
            }
            else if (parameterType == typeof(int16))
            {
                parameter = (*buffer)[pos++].Int16;
            }
            else if (parameterType == typeof(int32))
            {
                parameter = (*buffer)[pos++].Int32;
            }
            else if (parameterType == typeof(int64))
            {
                parameter = (*buffer)[pos++].Int64;
            }
            else if (parameterType == typeof(float))
            {
                parameter = (*buffer)[pos++].Float;
            }
            else if (parameterType == typeof(double))
            {
                parameter = (*buffer)[pos++].Double;
            }
            else if (parameterType == typeof(bool))
            {
                parameter = (*buffer)[pos++].Bool;
            }
            else if (parameterType.IsAssignableTo(typeof(IConjugate)))
            {
                parameter = (*buffer)[pos++].ReadConjugate<IConjugate>();
                if (parameter is not null && !parameter.GetType().IsAssignableTo(parameterType))
                {
                    return 2;
                }
            }
            else
            {
                return 3;
            }

            parameters.Add(parameter);
        }

        object? returnValue = Method.Invoke(obj, parameters.ToArray());
        if (Method.ReturnType != typeof(void))
        {
            Type returnType = Method.ReturnType;
            if (returnType == typeof(uint8))
            {
                (*buffer)[pos++].UInt8 = (uint8)returnValue!;
            }
            else if (returnType == typeof(uint16))
            {
                (*buffer)[pos++].UInt16 = (uint16)returnValue!;
            }
            else if (returnType == typeof(uint32))
            {
                (*buffer)[pos++].UInt32 = (uint32)returnValue!;
            }
            else if (returnType == typeof(uint64))
            {
                (*buffer)[pos++].UInt64 = (uint64)returnValue!;
            }
            else if (returnType == typeof(int8))
            {
                (*buffer)[pos++].Int8 = (int8)returnValue!;
            }
            else if (returnType == typeof(int16))
            {
                (*buffer)[pos++].Int16 = (int16)returnValue!;
            }
            else if (returnType == typeof(int32))
            {
                (*buffer)[pos++].Int32 = (int32)returnValue!;
            }
            else if (returnType == typeof(int64))
            {
                (*buffer)[pos++].Int64 = (int64)returnValue!;
            }
            else if (returnType == typeof(float))
            {
                (*buffer)[pos++].Float = (float)returnValue!;
            }
            else if (returnType == typeof(double))
            {
                (*buffer)[pos++].Double = (double)returnValue!;
            }
            else if (returnType == typeof(bool))
            {
                (*buffer)[pos++].Bool = (bool)returnValue!;
            }
            else if (returnType.IsAssignableTo(typeof(IConjugate)))
            {
                if (returnValue is not null && !returnValue.GetType().IsAssignableTo(returnType))
                {
                    return 4;
                }

                (*buffer)[pos++].WriteConjugate((IConjugate?)returnValue);
            }
            else
            {
                return 5;
            }
        }

        return 0;
	}

	public required string Name { get; init; }
	public required MethodInfo Method { get; init; }
	
}


