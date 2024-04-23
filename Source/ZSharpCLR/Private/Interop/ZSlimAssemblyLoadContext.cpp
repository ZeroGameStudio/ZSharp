// Copyright Zero Games. All Rights Reserved.


#include "ZSlimAssemblyLoadContext.h"

#include "ZSlimAssemblyLoadContext_Interop.h"

void ZSharp::FZSlimAssemblyLoadContext::Unload()
{
	FZSlimAssemblyLoadContext_Interop::GUnload(Handle);
}

int32 ZSharp::FZSlimAssemblyLoadContext::LoadAssembly(const TArray<uint8>& buffer, void* args)
{
	return FZSlimAssemblyLoadContext_Interop::GLoadAssembly(Handle, buffer.GetData(), buffer.Num(), args);
}

int32 ZSharp::FZSlimAssemblyLoadContext::CallMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args) const
{
	return FZSlimAssemblyLoadContext_Interop::GCallMethod(Handle, *assemblyName, *typeName, *methodName, args);
}
