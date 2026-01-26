// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class GameplayTag_Interop
{
	public static delegate* unmanaged<char*, char*, char*, char*, IntPtr> TryRegisterNativeTag;
	public static delegate* unmanaged<IntPtr, uint8> TryUnregisterNativeTag;
	
	public static delegate* unmanaged<char*, ConjugateHandle> Request;
}


