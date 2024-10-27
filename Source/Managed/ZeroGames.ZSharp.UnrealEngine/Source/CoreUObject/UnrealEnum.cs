// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealEnum
{

	public static bool IsUnrealEnumType(Type t) => t.IsEnum && t.GetCustomAttribute<UnrealFieldPathAttribute>() is not null;

	public static UnrealEnum GetUnrealEnum(Type t)
	{
		check(t.IsEnum);
		verify(t.GetCustomAttribute<UnrealFieldPathAttribute>() is var attr && attr is not null);
		return LowLevelFindObject<UnrealEnum>(attr.Path)!;
	}

}


