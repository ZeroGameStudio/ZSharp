// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealEnum
{

	public static bool IsUnrealEnumType(Type t) => t.IsEnum && t.GetCustomAttribute<UnrealFieldPathAttribute>() is not null;

	public static UnrealEnum GetUnrealEnum(Type t)
	{
		if (t.IsEnum && t.GetCustomAttribute<UnrealFieldPathAttribute>() is {} attr)
		{
			return UnrealObjectGlobals.LowLevelFindObject<UnrealEnum>(attr.Path)!;
		}

		throw new ArgumentException();
	}

}


