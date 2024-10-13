// Copyright Zero Games. All Rights Reserved.


#include "CustomThunkRegistry.h"

ZSharp::FCustomThunkRegistry& ZSharp::FCustomThunkRegistry::Get()
{
	static FCustomThunkRegistry GInstance;

	return GInstance;
}

FNativeFuncPtr ZSharp::FCustomThunkRegistry::GetThunk(FName name)
{
	return ThunkMap[name];
}

void ZSharp::FCustomThunkRegistry::Register(FName name, FNativeFuncPtr thunk)
{
	check(!ThunkMap.Contains(name));
	ThunkMap.Emplace(name, thunk);
}


