// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZWorld_Interop
	{
		static FZConjugateHandle SpawnActor(FZConjugateHandle self, FZConjugateHandle cls, FZConjugateHandle transform, const TCHAR* name, FZConjugateHandle actorTemplate, FZConjugateHandle owner, FZConjugateHandle instigator, FZConjugateHandle overrideLevel, ESpawnActorCollisionHandlingMethod spawnCollisionHandlingOverride, ESpawnActorScaleMethod transformScaleMethod, FActorSpawnParameters::ESpawnActorNameMode nameMode, uint8 absolute, uint8 deferred);
	};
}


