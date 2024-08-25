// Copyright Zero Games. All Rights Reserved.


#include "ZSlimAssemblyLoadContext.h"

#include "Interop/ZSlimAssemblyLoadContext_Interop.h"

void ZSharp::FZSlimAssemblyLoadContext::Unload()
{
	while (RunningCount)
	{
		FPlatformProcess::Sleep(0);
	}

	bUnloading = true;

	FZSlimAssemblyLoadContext_Interop::GUnload(Handle);
}

ZSharp::EZLoadAssemblyErrorCode ZSharp::FZSlimAssemblyLoadContext::LoadAssembly(const FString& assemblyName, void* args)
{
	if (bUnloading)
	{
		return EZLoadAssemblyErrorCode::AlcUnavailable;
	}
	
	++RunningCount;

	ON_SCOPE_EXIT { --RunningCount; };
	
	return FZSlimAssemblyLoadContext_Interop::GLoadAssembly(Handle, *assemblyName, args);
}

ZSharp::EZCallMethodErrorCode ZSharp::FZSlimAssemblyLoadContext::CallMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args)
{
	if (bUnloading)
	{
		return EZCallMethodErrorCode::AlcUnavailable;
	}
	
	++RunningCount;

	ON_SCOPE_EXIT { --RunningCount; };
	
	return FZSlimAssemblyLoadContext_Interop::GCallMethod(Handle, *assemblyName, *typeName, *methodName, args);
}
