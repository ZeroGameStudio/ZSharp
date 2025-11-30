// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class Profiling_Interop
{
	
	public static delegate* unmanaged[SuppressGCTransition]<char*, char*, int32, IntPtr> BeginCpuProfilerEvent;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, void> EndCpuProfilerEvent;

}


