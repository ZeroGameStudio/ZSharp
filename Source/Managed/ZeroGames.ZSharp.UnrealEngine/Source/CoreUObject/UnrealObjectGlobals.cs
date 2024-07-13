// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UnrealObjectGlobals
{

	public static UnrealObject? LoadObject(UnrealClass cls, UnrealObject? outer, string path) => ZCallEx.ZCall("ex://ObjectGlobals.LoadObject", cls, outer, new UnrealString(path), null)[3].ReadConjugate<UnrealObject>();

	public static T? LoadObject<T>(string path) where T : UnrealObject => LoadObject<T>(null, path);

	public static T? LoadObject<T>(UnrealObject? outer, string path) where T : UnrealObject => (T?)LoadObject((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!, outer, path);

	public static UnrealObject? FindObject(UnrealClass cls, UnrealObject? outer, string path, bool isExactClass) => ZCallEx.ZCall("ex://ObjectGlobals.FindObject", cls, outer, new UnrealString(path), isExactClass, null)[4].ReadConjugate<UnrealObject>();
	
	public static T? FindObject<T>(string path) where T : UnrealObject => FindObject<T>(null, path, false);

	public static T? FindObject<T>(UnrealObject? outer, string path, bool isExactClass) where T : UnrealObject => (T?)FindObject((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!, outer, path, isExactClass);

	public static T FindObjectChecked<T>(string path) where T : UnrealObject => FindObjectChecked<T>(null, path, false);
	
	public static T FindObjectChecked<T>(UnrealObject? outer, string path, bool isExactClass) where T : UnrealObject
	{
		T? res = FindObject<T>(outer, path, isExactClass);
		if (res is null)
		{
			throw new InvalidOperationException();
		}

		return res;
	}

	public static UnrealObject? LowLevelFindObject(string path) => ZCallEx.ZCall("ex://ObjectGlobals.LowLevelFindObject", new UnrealString(path), null)[1].ReadConjugate<UnrealObject>();

	public static T? LowLevelFindObject<T>(string path) where T : UnrealObject => LowLevelFindObject(path) as T;

}