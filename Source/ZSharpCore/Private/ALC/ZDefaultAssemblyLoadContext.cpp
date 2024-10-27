// Copyright Zero Games. All Rights Reserved.

#include "ZDefaultAssemblyLoadContext.h"

#include "Interop/Core/ZDefaultAssemblyLoadContext_Interop.h"

ZSharp::EZLoadAssemblyErrorCode ZSharp::FZDefaultAssemblyLoadContext::LoadAssembly(const FString& assemblyName, void* args)
{
	return FZDefaultAssemblyLoadContext_Interop::GLoadAssembly(*assemblyName, args);
}

ZSharp::EZInvokeMethodErrorCode ZSharp::FZDefaultAssemblyLoadContext::InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args)
{
	return FZDefaultAssemblyLoadContext_Interop::GInvokeMethod(*assemblyName, *typeName, *methodName, args);
}


