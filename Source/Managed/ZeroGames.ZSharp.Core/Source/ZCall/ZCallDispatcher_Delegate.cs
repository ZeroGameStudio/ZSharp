// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal class ZCallDispatcher_Delegate : IZCallDispatcher
{

	public const string KName = "d://";
	
	public unsafe int32 Dispatch(ZCallBuffer* buffer)
	{
		Delegate? @delegate = (*buffer)[0].GCHandle.Target as Delegate;
		if (@delegate is null)
		{
			return 1;
		}

		int32 pos = 1;
		MethodInfo method = @delegate.Method;
		ParameterInfo[] parameterInfos = method.GetParameters();
		List<object?> parameters = new();
		for (int32 i = 0; i < parameterInfos.Length; ++i)
		{
			parameters.Add((*buffer)[pos++].Object);
		}

		object? returnValue = @delegate.DynamicInvoke(parameters.ToArray());

		for (int32 i = 0; i < parameterInfos.Length; ++i)
		{
			var parameter = parameterInfos[i];
			if (parameter.IsOut)
			{
				(*buffer)[i + 1].Object = parameters[i];
			}
		}

		if (method.ReturnType != typeof(void))
		{
			(*buffer)[pos].Object = returnValue;
		}
		
		return 0;
	}

	public string Name => KName;

}


