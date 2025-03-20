// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UnrealObjectGlobals
{

	public static UnrealObject NewObject(UnrealClass @class, UnrealObject? outer, string? name)
	{
		MasterAlcCache.GuardInvariant();
		return InternalNewObject(@class, outer, name);
	}

	public static T NewObject<T>(UnrealClass @class) where T : UnrealObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(UnrealClass.FromType<T>(), null, null);
	}

	public static T NewObject<T>(UnrealClass @class, UnrealObject? outer) where T : UnrealObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(UnrealClass.FromType<T>(), outer, null);
	}

	public static T NewObject<T>(UnrealClass @class, string? name) where T : UnrealObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(UnrealClass.FromType<T>(), null, name);
	}

	public static T NewObject<T>(UnrealClass @class, UnrealObject? outer, string? name) where T : UnrealObject
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T)NewObject(UnrealClass.FromType<T>(), outer, name);
	}
	
	public static T NewObject<T>() where T : UnrealObject, IStaticClass
		=> NewObject<T>(T.StaticClass);
	public static T NewObject<T>(UnrealObject? outer) where T : UnrealObject, IStaticClass
		=> NewObject<T>(T.StaticClass, outer);
	public static T NewObject<T>(string? name) where T : UnrealObject, IStaticClass
		=> NewObject<T>(T.StaticClass, name);
	public static T NewObject<T>(UnrealObject? outer, string? name) where T : UnrealObject, IStaticClass
		=> NewObject<T>(T.StaticClass, outer, name);

	public static UnrealObject? LoadObject(UnrealClass @class, UnrealObject? outer, string? name)
	{
		MasterAlcCache.GuardInvariant();
		
		if (string.IsNullOrWhiteSpace(name))
		{
			return null;
		}
		
		return InternalLoadObject(@class, outer, name);
	}
	public static T? LoadObject<T>(string? name) where T : UnrealObject
		=> LoadObject<T>(null, name);
	public static T? LoadObject<T>(UnrealObject? outer, string? name) where T : UnrealObject
		=> (T?)LoadObject(UnrealClass.FromType<T>(), outer, name);
	
	public static UnrealObject DuplicateObject(UnrealObject source, UnrealObject outer, string? name, UnrealClass? @class)
	{
		MasterAlcCache.GuardInvariant();
		return InternalDuplicateObject(source, outer, name, @class);
	}
	public static T DuplicateObject<T>(UnrealObject source, UnrealObject outer) where T : UnrealObject
		=> (T)DuplicateObject(source, outer, null, null);
	public static T DuplicateObject<T>(UnrealObject source, UnrealObject outer, string? name) where T : UnrealObject
		=> (T)DuplicateObject(source, outer, name, null);
	public static T DuplicateObject<T>(UnrealObject source, UnrealObject outer, UnrealClass? @class) where T : UnrealObject
		=> (T)DuplicateObject(source, outer, null, @class);
	
	public static UnrealObject? FindObject(UnrealClass @class, UnrealObject? outer, string name, bool exactClass)
	{
		MasterAlcCache.GuardInvariant();
		return InternalFindObject(@class, outer, name, exactClass);
	}
	public static T? FindObject<T>(string name) where T : UnrealObject
		=> FindObject<T>(null, name, false);
	public static T? FindObject<T>(UnrealObject? outer, string name, bool exactClass) where T : UnrealObject
		=> (T?)FindObject(UnrealClass.FromType<T>(), outer, name, exactClass);

	public static T FindObjectChecked<T>(UnrealObject? outer, string name, bool exactClass) where T : UnrealObject
	{
		verify(FindObject<T>(outer, name, exactClass) is var res && res is not null);
		return res;
	}
	public static T FindObjectChecked<T>(string name) where T : UnrealObject
		=> FindObjectChecked<T>(null, name, false);

	public static UnrealObject? LowLevelFindObject(string path)
	{
		MasterAlcCache.GuardInvariant();
		return InternalLowLevelFindObject(path);
	}
	public static T? LowLevelFindObject<T>(string path) where T : UnrealObject
		=> LowLevelFindObject(path) as T;

	public static bool TryGetDefault<T>([NotNullWhen(true)] out T? result) where T : UnrealObject
	{
		bool exists = UnrealClass.FromType<T>().TryGetDefaultObject(out var cdo);
		result = (T?)cdo;
		return exists;
	}
	
	public static T GetDefault<T>() where T : UnrealObject
		=> (T)UnrealClass.FromType<T>().GetDefaultObject();

	public static Package GTransientPackage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalTransientPackage;
		}
	}

	private static unsafe UnrealObject InternalNewObject(UnrealClass @class, UnrealObject? outer, string? name)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.NewObject(ConjugateHandle.FromConjugate(@class), ConjugateHandle.FromConjugate(outer), nameBuffer).GetTargetChecked<UnrealObject>();
		}
	}

	private static unsafe UnrealObject? InternalLoadObject(UnrealClass @class, UnrealObject? outer, string name)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.LoadObject(ConjugateHandle.FromConjugate(@class), ConjugateHandle.FromConjugate(outer), nameBuffer).GetTarget<UnrealObject>();
		}
	}

	private static unsafe UnrealObject InternalDuplicateObject(UnrealObject source, UnrealObject outer, string? name, UnrealClass? @class)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.DuplicateObject(ConjugateHandle.FromConjugate(source), ConjugateHandle.FromConjugate(@class), nameBuffer, ConjugateHandle.FromConjugate(outer)).GetTargetChecked<UnrealObject>();
		}
	}

	private static unsafe UnrealObject? InternalFindObject(UnrealClass @class, UnrealObject? outer, string name, bool exactClass)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealObjectGlobals_Interop.FindObject(ConjugateHandle.FromConjugate(@class), ConjugateHandle.FromConjugate(outer), nameBuffer, Convert.ToByte(exactClass)).GetTarget<UnrealObject>();
		}
	}

	private static unsafe UnrealObject? InternalLowLevelFindObject(string path)
	{
		fixed (char* pathBuffer = path)
		{
			return UnrealObjectGlobals_Interop.LowLevelFindObject(pathBuffer).GetTarget<UnrealObject>();
		}
	}

	private static unsafe Package InternalTransientPackage => UnrealObjectGlobals_Interop.GetTransientPackage().GetTargetChecked<Package>();

}