// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	class ZSHARPCORE_API IZSlimAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual FString GetName() const = 0;
	public:
		virtual EZLoadAssemblyErrorCode LoadAssembly(const TArray<uint8>& buffer, void* args = nullptr) = 0;
	public:
		virtual EZCallMethodErrorCode CallMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args = nullptr) const = 0;
		// Helpers
	public:
		EZLoadAssemblyErrorCode LoadAssembly(const FString& path, void* args = nullptr);
	};
}


