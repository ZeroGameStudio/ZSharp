// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"
#include "ZCallHandle.h"

namespace ZSharp
{
	class IZAssembly;
	class IZType;
	class IZMethodInfo;
	class IZPropertyInfo;
	class IZConjugate;
	class IZCallDispatcher;
	
	class ZSHARPCLR_API IZMasterAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual IZAssembly* LoadAssembly(const TArray<uint8>& buffer) = 0;
		virtual IZAssembly* GetAssembly(const FString& name) const = 0;
	public:
		virtual IZType* GetType(const FString& name) const = 0;
	public:
		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) = 0;
		virtual IZCallDispatcher* GetZCallDispatcher(FZCallHandle handle) const = 0;
		virtual IZCallDispatcher* GetZCallDispatcher(const FString& name) const = 0;
		virtual FZCallHandle GetZCallHandle(const IZCallDispatcher* dispatcher) const = 0;
	};
}


