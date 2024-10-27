// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZGCHandle.h"
#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	class IZAssemblyLoadContext : public IZGCHandle
	{
	public:
		virtual void Unload() = 0;
	public:
		virtual EZLoadAssemblyErrorCode LoadAssembly(const FString& assemblyName, void* args = nullptr) = 0;
		virtual EZInvokeMethodErrorCode InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args = nullptr) = 0;
	};
}


