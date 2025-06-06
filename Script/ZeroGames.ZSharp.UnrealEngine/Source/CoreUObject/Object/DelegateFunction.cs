﻿// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UDelegateFunction
{
	
	public new static UDelegateFunction FromType(Type type)
	{
		PropertyInfo? staticUnrealFieldProperty = null;
		if (type.IsAssignableTo(typeof(IStaticSignature)))
		{
			staticUnrealFieldProperty = type.GetProperty(nameof(IStaticSignature.StaticSignature), BindingFlags.Public | BindingFlags.Static);
		}

		if (staticUnrealFieldProperty is null)
		{
			throw new ArgumentOutOfRangeException($"Type {type.FullName} is not a valid unreal field.");
		}
		
		return (UDelegateFunction)staticUnrealFieldProperty.GetValue(null)!;
	}
	public new static UDelegateFunction FromType<T>() => FromType(typeof(T));
	
}


