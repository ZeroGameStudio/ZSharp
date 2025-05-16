// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UScriptStruct
{
	
	public new static UScriptStruct FromType(Type type)
	{
		PropertyInfo? staticUnrealFieldProperty = null;
		if (type.IsAssignableTo(typeof(IStaticStruct)))
		{
			staticUnrealFieldProperty = type.GetProperty(nameof(IStaticStruct.StaticStruct), BindingFlags.Public | BindingFlags.Static);
		}

		if (staticUnrealFieldProperty is null)
		{
			throw new ArgumentOutOfRangeException($"Type {type.FullName} is not a valid unreal field.");
		}
		
		return (UScriptStruct)staticUnrealFieldProperty.GetValue(null)!;
	}
	public new static UScriptStruct FromType<T>() where T : IStaticStruct => FromType(typeof(T));
	
}


