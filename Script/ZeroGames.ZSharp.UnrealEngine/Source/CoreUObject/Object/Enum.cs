﻿// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UEnum
{

	public static UEnum FromType(Type type)
	{
		if (!type.IsEnum)
		{
			throw new ArgumentOutOfRangeException(nameof(type));
		}

		if (type.GetCustomAttribute<UnrealFieldPathAttribute>() is {} attr)
		{
			return LowLevelFindObject<UEnum>(attr.Path)!;
		}

		throw new ArgumentOutOfRangeException(nameof(type));
	}
	public static UEnum FromType<T>() where T : Enum  => FromType(typeof(T));
	
	public static bool IsUnrealEnumType(Type type) => type.IsEnum && type.GetCustomAttribute<UnrealFieldPathAttribute>() is not null;
	public static bool IsUnrealEnumType<T>() where T : Enum => IsUnrealEnumType(typeof(T));

}


