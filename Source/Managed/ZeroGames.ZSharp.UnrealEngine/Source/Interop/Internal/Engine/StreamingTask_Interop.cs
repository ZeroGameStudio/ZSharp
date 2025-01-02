// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class StreamingTask_Interop
{
	public static delegate* unmanaged<IntPtr, ConjugateHandle*, int32, int32> GetResult;
	public static delegate* unmanaged<IntPtr, int32> GetLoadedCount;
	public static delegate* unmanaged<IntPtr, void> Release;
}


