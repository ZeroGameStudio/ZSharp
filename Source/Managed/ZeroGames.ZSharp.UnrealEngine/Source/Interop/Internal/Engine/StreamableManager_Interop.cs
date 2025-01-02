// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class StreamableManager_Interop
{

	[UnmanagedCallersOnly]
	public static void Update(IntPtr unmanagedManager, IntPtr unmanagedTask, int32 loadedCount)
		=> StreamableManager.Get(unmanagedManager).Update(unmanagedTask, loadedCount);

	[UnmanagedCallersOnly]
	public static void SignalCompletion(IntPtr unmanagedManager, IntPtr unmanagedTask)
		=> StreamableManager.Get(unmanagedManager).SignalCompletion(unmanagedTask);

	public static delegate* unmanaged<IntPtr> GetGlobalStreamableManager;
	public static delegate* unmanaged<IntPtr, IntPtr, uint8, IntPtr> RequestAsyncLoading;
	
}


