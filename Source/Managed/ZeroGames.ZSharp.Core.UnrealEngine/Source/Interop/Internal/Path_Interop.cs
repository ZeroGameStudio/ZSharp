// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class Path_Interop
{
	
	public static delegate* unmanaged<IntPtr, void> GetProjectDir = null;
	public static delegate* unmanaged<char*, IntPtr, void> GetPluginDir = null;

}


