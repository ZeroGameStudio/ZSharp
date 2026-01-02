// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal sealed class ZCallDispatcher_Method(string name, MethodInfo method) : IZCallDispatcher
{
	
	public unsafe EZCallErrorCode Dispatch(ZCallBuffer* buffer)
	{
		List<IDisposable> marshaledObjects = [];
		try
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

			object? returnValue = Method.Invoke(obj, parameters);

			for (int32 i = 0; i < parameterInfos.Length; ++i)
			{
				var parameter = parameterInfos[i];
				if (parameter.ParameterType.IsByRef)
				{
					if (!parameter.ParameterType.IsEnum)
					{
						(*buffer)[Method.IsStatic ? i : i + 1].Object = parameters[i];
					}
					else
					{
						(*buffer)[Method.IsStatic ? i : i + 1].Int64 = (int64)Convert.ChangeType(parameters[i], typeof(int64))!;
					}
				}
			}

			if (Method.ReturnType != typeof(void))
			{
				if (!Method.ReturnType.IsEnum)
				{
					(*buffer)[pos].Object = returnValue;
				}
				else
				{
					(*buffer)[pos].Int64 = (int64)Convert.ChangeType(returnValue, typeof(int64))!;
				}
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

	public string Name { get; } = name;
	public MethodInfo Method { get; } = method;

}


