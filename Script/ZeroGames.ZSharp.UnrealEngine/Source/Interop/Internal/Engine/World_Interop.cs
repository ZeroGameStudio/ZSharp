// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class World_Interop
{
	[UnmanagedCallersOnly]
	public static void NotifyWorldInitialized(ConjugateHandle world)
		=> UWorld.InternalNotifyWorldInitialized(world);
	
	[UnmanagedCallersOnly]
	public static void NotifyWorldTearingDown(ConjugateHandle world)
		=> UWorld.InternalNotifyWorldTearingDown(world);
	
	[UnmanagedCallersOnly]
	public static void PreLoadMap(char* map)
		=> UWorld.InternalPreLoadMap(new(map));
	
	[UnmanagedCallersOnly]
	public static void PostLoadMap(ConjugateHandle world)
		=> UWorld.InternalPostLoadMap(world);
	
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, ConjugateHandle, char*, ConjugateHandle, ConjugateHandle, ConjugateHandle, ConjugateHandle, ESpawnActorCollisionHandlingMethod, ESpawnActorScaleMethod, ESpawnActorNameMode, uint8, uint8, ConjugateHandle> SpawnActor;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, EWorldType> GetWorldType;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ENetMode> GetNetMode;
}


