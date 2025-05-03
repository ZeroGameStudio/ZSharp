// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZWorld_Interop
	{
		static FZConjugateHandle SpawnActor(FZConjugateHandle self, FZConjugateHandle cls, FZConjugateHandle transform, const TCHAR* name, FZConjugateHandle actorTemplate, FZConjugateHandle owner, FZConjugateHandle instigator, FZConjugateHandle overrideLevel, ESpawnActorCollisionHandlingMethod spawnCollisionHandlingOverride, ESpawnActorScaleMethod transformScaleMethod, FActorSpawnParameters::ESpawnActorNameMode nameMode, uint8 absolute, uint8 deferred);
		static EWorldType::Type GetWorldType(FZConjugateHandle self);
		static ENetMode GetNetMode(FZConjugateHandle self);

		inline static void(*GNotifyWorldInitialized)(UWorld*) = nullptr;
		inline static void(*GNotifyWorldTearingDown)(UWorld*) = nullptr;
		inline static void(*GPreLoadMap)(const TCHAR*) = nullptr;
		inline static void(*GPostLoadMap)(UWorld*) = nullptr;
	};
}


