// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UStruct
{

	public static UStruct FromType(Type type)
	{
		PropertyInfo? staticUnrealFieldProperty = null;
		if (type.IsAssignableTo(typeof(IUnrealObject)))
		{
			// classes or interfaces
			staticUnrealFieldProperty = type.GetProperty(nameof(IStaticClass.StaticClass), BindingFlags.Public | BindingFlags.Static);
		}
		else if (type.IsAssignableTo(typeof(IStaticStruct)))
		{
			// structs
			staticUnrealFieldProperty = type.GetProperty(nameof(IStaticStruct.StaticStruct), BindingFlags.Public | BindingFlags.Static);
		}
		else if (type.IsAssignableTo(typeof(IStaticSignature)))
		{
			// delegates
			staticUnrealFieldProperty = type.GetProperty(nameof(IStaticSignature.StaticSignature), BindingFlags.Public | BindingFlags.Static);
		}

		if (staticUnrealFieldProperty is null)
		{
			throw new ArgumentOutOfRangeException($"Type {type.FullName} is not a valid unreal field.");
		}
		
		return (UStruct)staticUnrealFieldProperty.GetValue(null)!;
	}
	public static UStruct FromType<T>() => FromType(typeof(T));

	public bool IsChildOf(UStruct other)
	{
		MasterAlcCache.GuardInvariant();
		return InternalIsChildOf(other);
	}
	public bool IsChildOf(Type other) => IsChildOf(FromType(other));
	public bool IsChildOf<T>() => IsChildOf(typeof(T));

	private unsafe bool InternalIsChildOf(UStruct other) => UnrealStruct_Interop.IsChildOf(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;

}


