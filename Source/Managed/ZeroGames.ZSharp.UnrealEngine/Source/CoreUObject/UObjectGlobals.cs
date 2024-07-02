// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UObjectGlobals
{
	
	public static T? FindObject<T>(string path) where T : UnrealObject => FindObject<T>(null, path, false);
	
	public static T? FindObject<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject
	{
		throw new NotImplementedException();
	}

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

}