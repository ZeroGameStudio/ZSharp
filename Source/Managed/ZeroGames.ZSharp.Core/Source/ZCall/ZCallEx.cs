// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class ZCallEx
{

	public static int32 ZCall(this object @this, ZCallHandle handle, params object[] parameters)
	{
		return 0;
	}
	
	public static int32 ZCall(this object @this, string name, params object[] parameters)
	{
		return 0;
	}
	
	public static int32 ZCall(this object @this, string name, out ZCallHandle handle, params object[] parameters)
	{
		handle = new();
		return 0;
	}
	
	public static int32 ZCall(ZCallHandle handle, params object[] parameters)
	{
		return 0;
	}
	
	public static int32 ZCall(string name, params object[] parameters)
	{
		return 0;
	}
	
	public static int32 ZCall(string name, out ZCallHandle handle, params object[] parameters)
	{
		handle = new();
		return 0;
	}
	
}


