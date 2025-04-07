// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal sealed class ZCallDispatcher_Method(string name, MethodInfo method) : IZCallDispatcher
{
	
	public unsafe EZCallErrorCode Dispatch(ZCallBuffer* buffer)
	{
        int32 pos = 0;
        
        object? obj = null;
        if (!Method.IsStatic)
        {
            Type thisType = Method.DeclaringType!;
            obj = (*buffer)[pos++].ReadConjugate<IConjugate>();
            if (obj is null || !obj.GetType().IsAssignableTo(thisType))
            {
                return EZCallErrorCode.BufferError;
            }
        }

        ParameterInfo[] parameterInfos = Method.GetParameters();
        object?[] parameters = new object?[parameterInfos.Length];
        for (int32 i = 0; i < parameterInfos.Length; ++i)
        {
	        Type parameterType = parameterInfos[i].ParameterType;
	        parameters[i] = parameterType.IsEnum ? Convert.ChangeType((*buffer)[pos++].Object, parameterType.GetEnumUnderlyingType()) : (*buffer)[pos++].Object;
        }
        
        object? returnValue = Method.Invoke(obj, parameters);
        
        for (int32 i = 0; i < parameterInfos.Length; ++i)
        {
	        var parameter = parameterInfos[i];
	        if (parameter.ParameterType.IsByRef)
	        {
		        (*buffer)[Method.IsStatic ? i : i + 1].Object = parameters[i];
	        }
        }
        
        if (Method.ReturnType != typeof(void))
        {
            (*buffer)[pos].Object = returnValue;
        }

        return EZCallErrorCode.Succeed;
	}

	public string Name { get; } = name;
	public MethodInfo Method { get; } = method;

}


