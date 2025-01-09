// Copyright Zero Games. All Rights Reserved.

#include "ZCustomThunkRegistry.h"

ZSharp::FZCustomThunkRegistry& ZSharp::FZCustomThunkRegistry::Get()
{
	static FZCustomThunkRegistry GInstance;

	return GInstance;
}

FNativeFuncPtr ZSharp::FZCustomThunkRegistry::GetThunk(FName name)
{
	return ThunkMap[name];
}

void ZSharp::FZCustomThunkRegistry::Register(FName name, FNativeFuncPtr thunk)
{
	check(!ThunkMap.Contains(name));
	ThunkMap.Emplace(name, thunk);
}


