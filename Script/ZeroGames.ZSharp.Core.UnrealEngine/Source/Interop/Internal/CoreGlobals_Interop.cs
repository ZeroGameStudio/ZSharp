// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class CoreGlobals_Interop
{

	public static delegate* unmanaged[SuppressGCTransition]<uint8> IsInGameThreadFuncPtr;

	public static uint8* GIsServerPtr;
	public static uint8* GIsClientPtr;
	public static uint8* GIsEditorPtr;
	public static uint64* GFrameCounterPtr;

}


