// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class Config_Interop
{
	
	public static delegate* unmanaged<IntPtr, char*, IntPtr, void> GetFileName = null;
	public static delegate* unmanaged<IntPtr, char*, char*, IntPtr, uint8> TryGetSection = null;
	public static delegate* unmanaged<IntPtr, char*, char*, char*, IntPtr, uint8> TryGetArray = null;
	public static delegate* unmanaged<IntPtr, char*, char*, char*, IntPtr, uint8> TryGetString = null;
	
}


