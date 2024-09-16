// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal class ZCallDispatcher_Method : IZCallDispatcher
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
	        parameters[i] = (*buffer)[pos++].Object;
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

	public required string Name { get; init; }
	public required MethodInfo Method { get; init; }
	
}


