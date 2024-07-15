// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal class ZCallDispatcher_Method : IZCallDispatcher
{
	
	public unsafe int32 Dispatch(ZCallBuffer* buffer)
	{
        int32 pos = 0;
        
        object? obj = null;
        if (!Method.IsStatic)
        {
            Type thisType = Method.DeclaringType!;
            obj = (*buffer)[pos++].ReadConjugate<IConjugate>();
            if (obj is null || !obj.GetType().IsAssignableTo(thisType))
            {
                return 1;
            }
        }

        ParameterInfo[] parameterInfos = Method.GetParameters();
        List<object?> parameters = new();
        for (int32 i = 0; i < parameterInfos.Length; ++i)
        {
            parameters.Add((*buffer)[pos++].Object);
        }

        object? returnValue = Method.Invoke(obj, parameters.ToArray());
        
        for (int32 i = 0; i < parameterInfos.Length; ++i)
        {
	        var parameter = parameterInfos[i];
	        if (parameter.IsOut)
	        {
		        (*buffer)[Method.IsStatic ? i : i + 1].Object = parameters[i];
	        }
        }
        
        if (Method.ReturnType != typeof(void))
        {
            (*buffer)[pos].Object = returnValue;
        }

        return 0;
	}

	public required string Name { get; init; }
	public required MethodInfo Method { get; init; }
	
}


