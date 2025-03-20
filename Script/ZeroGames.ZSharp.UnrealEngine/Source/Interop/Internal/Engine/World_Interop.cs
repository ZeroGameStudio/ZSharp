// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class World_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, ConjugateHandle, char*, ConjugateHandle, ConjugateHandle, ConjugateHandle, ConjugateHandle, ESpawnActorCollisionHandlingMethod, ESpawnActorScaleMethod, ESpawnActorNameMode, uint8, uint8, ConjugateHandle> SpawnActor;
	public static delegate* unmanaged<ConjugateHandle, EWorldType> GetWorldType;
	public static delegate* unmanaged<ConjugateHandle, ENetMode> GetNetMode;
}


