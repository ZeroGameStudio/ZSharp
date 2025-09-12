// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UnrealObjectGlobals
{

	public static UObject NewObject(UClass @class, UObject? outer, string? name)
	{
		MasterAlcCache.GuardInvariant();
		return InternalNewObject(@class, outer, name);
	}

	public static T NewObject<T>(UClass @class) where T : UObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(@class, null, null);
	}

	public static T NewObject<T>(UClass @class, UObject? outer) where T : UObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(@class, outer, null);
	}

	public static T NewObject<T>(UClass @class, string? name) where T : UObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(@class, null, name);
	}

	public static T NewObject<T>(UClass @class, UObject? outer, string? name) where T : UObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(@class, outer, name);
	}
	
	public static T NewObject<T>() where T : UObject, IStaticClass
		=> NewObject<T>(T.StaticClass);
	public static T NewObject<T>(UObject? outer) where T : UObject, IStaticClass
		=> NewObject<T>(T.StaticClass, outer);
	public static T NewObject<T>(string? name) where T : UObject, IStaticClass
		=> NewObject<T>(T.StaticClass, name);
	public static T NewObject<T>(UObject? outer, string? name) where T : UObject, IStaticClass
		=> NewObject<T>(T.StaticClass, outer, name);

	public static UObject? LoadObject(UClass @class, UObject? outer, string? name)
	{
		MasterAlcCache.GuardInvariant();
		
		if (string.IsNullOrWhiteSpace(name))
		{
			return null;
		}
		
		return InternalLoadObject(@class, outer, name);
	}
	public static T? LoadObject<T>(string? name) where T : UObject
		=> LoadObject<T>(null, name);
	public static T? LoadObject<T>(UObject? outer, string? name) where T : UObject
		=> (T?)LoadObject(UClass.FromType<T>(), outer, name);
	
	public static UObject DuplicateObject(UObject source, UObject outer, string? name, UClass? @class)
	{
		MasterAlcCache.GuardInvariant();
		return InternalDuplicateObject(source, outer, name, @class);
	}
	public static T DuplicateObject<T>(UObject source, UObject outer) where T : UObject
		=> (T)DuplicateObject(source, outer, null, null);
	public static T DuplicateObject<T>(UObject source, UObject outer, string? name) where T : UObject
		=> (T)DuplicateObject(source, outer, name, null);
	public static T DuplicateObject<T>(UObject source, UObject outer, UClass? @class) where T : UObject
		=> (T)DuplicateObject(source, outer, null, @class);
	
	public static UObject? FindObject(UClass @class, UObject? outer, string name, bool exactClass)
	{
		MasterAlcCache.GuardInvariant();
		return InternalFindObject(@class, outer, name, exactClass);
	}
	public static T? FindObject<T>(string name) where T : UObject
		=> FindObject<T>(null, name, false);
	public static T? FindObject<T>(UObject? outer, string name, bool exactClass) where T : UObject
		=> (T?)FindObject(UClass.FromType<T>(), outer, name, exactClass);

	public static T FindObjectChecked<T>(UObject? outer, string name, bool exactClass) where T : UObject
	{
		verify(FindObject<T>(outer, name, exactClass) is var res && res is not null);
		return res;
	}
	public static T FindObjectChecked<T>(string name) where T : UObject
		=> FindObjectChecked<T>(null, name, false);

	public static UObject? LowLevelFindObject(string path)
	{
		MasterAlcCache.GuardInvariant();
		return InternalLowLevelFindObject(path);
	}
	public static T? LowLevelFindObject<T>(string path) where T : UObject
		=> LowLevelFindObject(path) as T;

	public static bool TryGetDefault<T>([NotNullWhen(true)] out T? result) where T : UObject
	{
		bool exists = UClass.FromType<T>().TryGetDefaultObject(out var cdo);
		result = (T?)cdo;
		return exists;
	}
	
	public static T GetDefault<T>() where T : UObject
		=> (T)UClass.FromType<T>().GetDefaultObject();

	public static UPackage GTransientPackage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalTransientPackage;
		}
	}

	private static unsafe UObject InternalNewObject(UClass @class, UObject? outer, string? name)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.NewObject(ConjugateHandle.FromConjugate(@class), ConjugateHandle.FromConjugate(outer), nameBuffer).GetTargetChecked<UObject>();
		}
	}

	private static unsafe UObject? InternalLoadObject(UClass @class, UObject? outer, string name)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.LoadObject(ConjugateHandle.FromConjugate(@class), ConjugateHandle.FromConjugate(outer), nameBuffer).GetTarget<UObject>();
		}
	}

	private static unsafe UObject InternalDuplicateObject(UObject source, UObject outer, string? name, UClass? @class)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.DuplicateObject(ConjugateHandle.FromConjugate(source), ConjugateHandle.FromConjugate(@class), nameBuffer, ConjugateHandle.FromConjugate(outer)).GetTargetChecked<UObject>();
		}
	}

	private static unsafe UObject? InternalFindObject(UClass @class, UObject? outer, string name, bool exactClass)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.FindObject(ConjugateHandle.FromConjugate(@class), ConjugateHandle.FromConjugate(outer), nameBuffer, Convert.ToByte(exactClass)).GetTarget<UObject>();
		}
	}

	private static unsafe UObject? InternalLowLevelFindObject(string path)
	{
		fixed (char* pathBuffer = path)
		{
			return UnrealObjectGlobals_Interop.LowLevelFindObject(pathBuffer).GetTarget<UObject>();
		}
	}

	private static unsafe UPackage InternalTransientPackage => UnrealObjectGlobals_Interop.GetTransientPackage().GetTargetChecked<UPackage>();

}