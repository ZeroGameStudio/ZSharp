// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal sealed class ZCallDispatcher_Delegate : IZCallDispatcher
{
	
	public unsafe EZCallErrorCode Dispatch(ZCallBuffer* buffer)
	{
		List<IDisposable> marshaledObjects = [];
		try
		{
			Delegate? @delegate = (*buffer)[0].GCHandle.Target as Delegate;
			if (@delegate is null)
			{
				return EZCallErrorCode.BufferError;
			}
	
			int32 pos = 1;
			MethodInfo method = @delegate.Method;
			ParameterInfo[] parameterInfos = method.GetParameters();
			object?[] parameters = new object?[parameterInfos.Length];
			for (int32 i = 0; i < parameterInfos.Length; ++i)
			{
				Type parameterType = parameterInfos[i].ParameterType;
				if (parameterType.IsEnum)
				{
					parameters[i] = Convert.ChangeType((*buffer)[pos++].Object, parameterType.GetEnumUnderlyingType());
				}
				else if (parameterType.IsAssignableTo(typeof(IMarshalPointer)))
				{
					var marshaledObject = (IDisposable)parameterType.GetMethod(nameof(IMarshalPointer.ToManaged), BindingFlags.Public | BindingFlags.Static)!.Invoke(null, [(*buffer)[pos++].Pointer])!;
					parameters[i] = marshaledObject;
					marshaledObjects.Add(marshaledObject);
				}
				else
				{
					parameters[i] = (*buffer)[pos++].Object;
				}
			}
	
			object? returnValue = @delegate.DynamicInvoke(parameters);
	
			for (int32 i = 0; i < parameterInfos.Length; ++i)
			{
				var parameter = parameterInfos[i];
				if (parameter.ParameterType.IsByRef)
				{
					(*buffer)[i + 1].Object = parameters[i];
				}
			}
	
			if (method.ReturnType != typeof(void))
			{
				(*buffer)[pos].Object = returnValue;
			}
			
			return EZCallErrorCode.Succeed;
		}
		finally
		{
			foreach (var marshaledObject in marshaledObjects)
			{
				marshaledObject.Dispose();
			}
		}
	}

	public string Name => ZCALL_NAME;
	
	private const string ZCALL_NAME = "nd://";

}


