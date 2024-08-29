// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UnrealObjectGlobals
{

	public static UnrealClass GetClass(Type type)
	{
		if (!type.IsAssignableTo(typeof(IUnrealObject)))
		{
			throw new ArgumentOutOfRangeException(nameof(type));
		}

		return GetClassUnchecked(type);
	}
	
	public static UnrealClass GetClass<T>() where T : IUnrealObject => GetClassUnchecked(typeof(T));

	public static UnrealObject? LoadObject(UnrealClass cls, UnrealObject? outer, string path) => ZCallEx.ZCall("ex://ObjectGlobals.LoadObject", cls, outer, new UnrealString(path), null)[3].ReadConjugate<UnrealObject>();

	public static T? LoadObject<T>(string path) where T : UnrealObject => LoadObject<T>(null, path);

	public static T? LoadObject<T>(UnrealObject? outer, string path) where T : UnrealObject => (T?)LoadObject(GetClass<T>(), outer, path);

	public static UnrealObject? FindObject(UnrealClass cls, UnrealObject? outer, string path, bool exactClass) => ZCallEx.ZCall("ex://ObjectGlobals.FindObject", cls, outer, new UnrealString(path), exactClass, null)[4].ReadConjugate<UnrealObject>();
	
	public static T? FindObject<T>(string path) where T : UnrealObject => FindObject<T>(null, path, false);

	public static T? FindObject<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject => (T?)FindObject(GetClass<T>(), outer, path, exactClass);

	public static T FindObjectChecked<T>(string path) where T : UnrealObject => FindObjectChecked<T>(null, path, false);
	
	public static T FindObjectChecked<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject
	{
		T? res = FindObject<T>(outer, path, exactClass);
		if (res is null)
		{
			throw new InvalidOperationException();
		}

		return res;
	}

	public static UnrealObject? LowLevelFindObject(string path) => ZCallEx.ZCall("ex://ObjectGlobals.LowLevelFindObject", new UnrealString(path), null)[1].ReadConjugate<UnrealObject>();

	public static T? LowLevelFindObject<T>(string path) where T : UnrealObject => LowLevelFindObject(path) as T;

	private static UnrealClass GetClassUnchecked(Type type) => (UnrealClass)type.GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!;
	
}