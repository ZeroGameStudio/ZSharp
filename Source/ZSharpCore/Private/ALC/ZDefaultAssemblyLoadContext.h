// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZDefaultAssemblyLoadContext.h"

namespace ZSharp
{
	class FZDefaultAssemblyLoadContext final : public IZDefaultAssemblyLoadContext
	{

	public:
		virtual EZLoadAssemblyErrorCode LoadAssembly(const FString& assemblyName, void* args) override;
		virtual EZInvokeMethodErrorCode InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args) override;
		
	};
}


