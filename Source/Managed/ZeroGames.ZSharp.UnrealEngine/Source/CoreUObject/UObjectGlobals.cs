// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UObjectGlobals
{

	public static UnrealObject? FindObject(UnrealClass cls, UnrealObject? outer, string path, bool exactClass) => ZCallEx.ZCall("ex://UObjectGlobals.FindObject", cls, outer, new UnrealString(path), exactClass, null)[4].ReadConjugate<UnrealObject>();
	
	public static T? FindObject<T>(string path) where T : UnrealObject => FindObject<T>(null, path, false);

	public static T? FindObject<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject => (T?)FindObject((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!, outer, path, exactClass);

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

	public static UnrealObject? LowLevelFindObject(string path) => ZCallEx.ZCall("ex://UObjectGlobals.LowLevelFindObject", new UnrealString(path), null)[1].ReadConjugate<UnrealObject>();

	public static T? LowLevelFindObject<T>(string path) where T : UnrealObject => LowLevelFindObject(path) as T;

}