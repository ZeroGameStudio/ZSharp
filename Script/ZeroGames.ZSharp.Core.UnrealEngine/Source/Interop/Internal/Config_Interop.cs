// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class Config_Interop
{
	
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, char*, IntPtr, void> GetFileName;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, char*, char*, IntPtr, uint8> TryGetSection;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, char*, char*, char*, IntPtr, uint8> TryGetArray;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, char*, char*, char*, IntPtr, uint8> TryGetString;
	
}


