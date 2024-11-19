// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class CoreGlobals_Interop
{

	public static delegate* unmanaged<uint8> IsInGameThreadFuncPtr;

	public static uint8* GIsServerPtr;
	public static uint8* GIsClientPtr;
	public static uint8* GIsEditorPtr;
	public static uint64* GFrameCounterPtr;

}


