// Copyright Zero Games. All Rights Reserved.

#include "ZSlimAssemblyLoadContext.h"

#include "Interop/Core/ZSlimAssemblyLoadContext_Interop.h"

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

ZSharp::EZInvokeMethodErrorCode ZSharp::FZSlimAssemblyLoadContext::InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args)
{
	if (bUnloading)
	{
		return EZInvokeMethodErrorCode::AlcUnavailable;
	}
	
	++RunningCount;

	ON_SCOPE_EXIT { --RunningCount; };
	
	return FZSlimAssemblyLoadContext_Interop::GInvokeMethod(Handle, *assemblyName, *typeName, *methodName, args);
}
