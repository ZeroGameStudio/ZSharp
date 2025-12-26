// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using ZeroGames.Extensions.Math;

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class World_Interop
{
	[UnmanagedCallersOnly]
	public static void NotifyWorldInitialized(ConjugateHandle world)
	{
		try
		{
			UWorld.InternalNotifyWorldInitialized(world);
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex);
		}
	}

	[UnmanagedCallersOnly]
	public static void NotifyWorldTearingDown(ConjugateHandle world)
	{
		try
		{
			UWorld.InternalNotifyWorldTearingDown(world);
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex);
		}
	}

	[UnmanagedCallersOnly]
	public static void PreLoadMap(char* map)
	{
		try
		{
			UWorld.InternalPreLoadMap(new(map));
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex);
		}
	}

	[UnmanagedCallersOnly]
	public static void PostLoadMap(ConjugateHandle world)
	{
		try
		{
			UWorld.InternalPostLoadMap(world);
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex);
		}
	}
	
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, in Transform, char*, ConjugateHandle, ConjugateHandle, ConjugateHandle, ConjugateHandle, ESpawnActorCollisionHandlingMethod, ESpawnActorScaleMethod, ESpawnActorNameMode, uint8, uint8, ConjugateHandle> SpawnActor;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, EWorldType> GetWorldType;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ENetMode> GetNetMode;
}


