// Copyright Zero Games. All Rights Reserved.

#include "ZSharpClassInstanceRegistry.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

#if ZSHARP_WITH_MASTER_ALC_RELOAD

ZSharp::FZSharpClassInstanceRegistry& ZSharp::FZSharpClassInstanceRegistry::Get()
{
	static FZSharpClassInstanceRegistry GInstance;

	return GInstance;
}

ZSharp::FZSharpClassInstanceRegistry::FZSharpClassInstanceRegistry()
{
	// The first time master ALC loaded is before UObject system initialized so there is no need to call CallOrRegisterOnMasterAlcLoaded().
	IZSharpClr::Get().OnMasterAlcLoaded().AddRaw(this, &ThisClass::HandleMasterAlcReloaded);
}

void ZSharp::FZSharpClassInstanceRegistry::Register(UObject* instance)
{
	// @FIXME: Multithread loading should be disabled if master ALC reloading is enabled, but this can change in the future.
	check(IsInGameThread());
	
	bool alreadyExists;
	Instances.Emplace(instance, &alreadyExists);
	check(!alreadyExists);
}

void ZSharp::FZSharpClassInstanceRegistry::HandleMasterAlcReloaded(IZMasterAssemblyLoadContext* alc)
{
	auto& registry = alc->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	TSet<FObjectKey> validInstances;
	for (auto& instanceKey : Instances)
	{
		if (UObject* instance = instanceKey.ResolveObjectPtr())
		{
			if (registry.CanBuildConjugate(instance))
			{
				// Try build conjugate to trigger managed constructor.
				registry.Conjugate(instance);
				validInstances.Emplace(instance);
			}
		}
	}
	Instances = MoveTemp(validInstances);
}

#endif


