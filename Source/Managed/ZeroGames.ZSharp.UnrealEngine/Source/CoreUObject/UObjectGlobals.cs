// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UObjectGlobals
{

	public static UnrealClass GetStaticClass(Type type)
	{
		check(type.IsAssignableTo(typeof(IUnrealObject)));
		return GetClassUnchecked(type);
	}
	
	public static UnrealClass GetStaticClass<T>() where T : IUnrealObject => GetClassUnchecked(typeof(T));

	public static UnrealObject NewObject(UnrealClass cls, UnrealObject? outer, string? name) => DynamicZCall.ZCall("ex://ObjectGlobals.NewObject", cls, outer, name is not null ? new UnrealName(name) : null, null)[3].ReadConjugate<UnrealObject>()!;

	public static T NewObject<T>() where T : UnrealObject => (T)NewObject(GetStaticClass<T>(), null, null);
	
	public static T NewObject<T>(UnrealObject? outer) where T : UnrealObject => (T)NewObject(GetStaticClass<T>(), outer, null);
	
	public static T NewObject<T>(string? name) where T : UnrealObject => (T)NewObject(GetStaticClass<T>(), null, name);
	
	public static T NewObject<T>(UnrealObject? outer, string? name) where T : UnrealObject => (T)NewObject(GetStaticClass<T>(), outer, name);

	public static UnrealObject? LoadObject(UnrealClass cls, UnrealObject? outer, string path) => DynamicZCall.ZCall("ex://ObjectGlobals.LoadObject", cls, outer, new UnrealString(path), null)[3].ReadConjugate<UnrealObject>();

	public static T? LoadObject<T>(string path) where T : UnrealObject => LoadObject<T>(null, path);

	public static T? LoadObject<T>(UnrealObject? outer, string path) where T : UnrealObject => (T?)LoadObject(GetStaticClass<T>(), outer, path);

	public static UnrealObject? FindObject(UnrealClass cls, UnrealObject? outer, string path, bool exactClass) => DynamicZCall.ZCall("ex://ObjectGlobals.FindObject", cls, outer, new UnrealString(path), exactClass, null)[4].ReadConjugate<UnrealObject>();
	
	public static T? FindObject<T>(string path) where T : UnrealObject => FindObject<T>(null, path, false);

	public static T? FindObject<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject => (T?)FindObject(GetStaticClass<T>(), outer, path, exactClass);

	public static T FindObjectChecked<T>(string path) where T : UnrealObject => FindObjectChecked<T>(null, path, false);
	
	public static T FindObjectChecked<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject
	{
		T? res = FindObject<T>(outer, path, exactClass);
		
		check(res is not null);
		return res;
	}

	public static UnrealObject? LowLevelFindObject(string path) => DynamicZCall.ZCall("ex://ObjectGlobals.LowLevelFindObject", new UnrealString(path), null)[1].ReadConjugate<UnrealObject>();

	public static T? LowLevelFindObject<T>(string path) where T : UnrealObject => LowLevelFindObject(path) as T;

	private static UnrealClass GetClassUnchecked(Type type) => (UnrealClass)type.GetProperty(nameof(IStaticClass.StaticClass))!.GetValue(null)!;
	
}