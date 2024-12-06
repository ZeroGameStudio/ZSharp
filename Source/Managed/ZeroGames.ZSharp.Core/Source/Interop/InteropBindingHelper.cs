// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace ZeroGames.ZSharp.Core;

public static class InteropBindingHelper
{

	public static FieldInfo GetStaticFunctionPointerField(string typeName, string fieldName)
	{
		StackFrame frame = new(1);
		Type? type = frame.GetMethod()!.DeclaringType!.Assembly.GetType(typeName);
		if (type is null)
		{
			Debugger.Break();
			Throw($"Binding failed, type not found. {typeName}");
		}
            
		FieldInfo? field = type.GetField(fieldName, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static);
		if (field is null)
		{
			Debugger.Break();
			Throw($"Binding failed, field not found. {typeName}.{fieldName}");
		}

		if (!field.FieldType.IsUnmanagedFunctionPointer)
		{
			Debugger.Break();
			Throw($"Binding failed, field is not unmanaged function pointer. {typeName}.{fieldName}");
		}

		return field;
	}

	[DoesNotReturn]
	private static void Throw(string message)
	{
		throw new FatalException(message);
	}
	
}


