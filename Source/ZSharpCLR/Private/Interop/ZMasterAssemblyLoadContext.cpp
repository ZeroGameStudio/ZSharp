// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext.h"

void ZSharp::FZMasterAssemblyLoadContext::Release()
{
	Unload();
}

ZSharp::FZGCHandle ZSharp::FZMasterAssemblyLoadContext::GetGCHandle() const
{
	return Handle;
}

void ZSharp::FZMasterAssemblyLoadContext::Unload()
{
	checkNoEntry();
}

ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::LoadAssembly(const FString& name, TArray<uint8> buffer)
{
	checkNoEntry();
	return nullptr;
}

ZSharp::IZAssembly* ZSharp::FZMasterAssemblyLoadContext::GetAssembly(const FString& name) const
{
	checkNoEntry();
	return nullptr;
}
