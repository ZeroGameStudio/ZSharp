// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class Path_Interop
{
	
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, void> GetProjectDir;
	public static delegate* unmanaged[SuppressGCTransition]<char*, IntPtr, void> GetPluginDir;

}


