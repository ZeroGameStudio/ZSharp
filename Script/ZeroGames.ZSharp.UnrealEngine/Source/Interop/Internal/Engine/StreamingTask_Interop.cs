// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class StreamingTask_Interop
{
	public static delegate* unmanaged<IntPtr, ConjugateHandle*, int32, int32> GetResult;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int32> GetLoadedCount;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, void> Release;
}


