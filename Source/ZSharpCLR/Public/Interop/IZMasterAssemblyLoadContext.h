// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"
#include "ZCallHandle.h"
#include "ZConjugateHandle.h"

namespace ZSharp
{
	class IZAssembly;
	class IZType;
	class IZMethodInfo;
	class IZPropertyInfo;
	class IZCallDispatcher;
	class IZCallResolver;
	
	class ZSHARPCLR_API IZMasterAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual const IZAssembly* LoadAssembly(const TArray<uint8>& buffer, void* args = nullptr) = 0;
		virtual const IZAssembly* GetAssembly(const FString& name) const = 0;
	public:
		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) = 0;
		virtual const IZCallDispatcher* GetZCallDispatcher(FZCallHandle handle) const = 0;
		virtual const IZCallDispatcher* GetZCallDispatcher(const FString& name) const = 0;
		virtual const IZCallDispatcher* GetOrResolveZCallDispatcher(const FString& name) = 0;
		virtual FZCallHandle GetZCallHandle(const IZCallDispatcher* dispatcher) const = 0;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) = 0;
	public:
		virtual FZConjugateHandle BuildConjugate(void* unmanaged, const IZType* managedType) = 0;
		virtual void BuildConjugate(void* unmanaged, FZConjugateHandle managed) = 0;
		virtual void ReleaseConjugate(void* unmanaged) = 0;
		virtual void ReleaseConjugate(FZConjugateHandle managed) = 0;
		virtual FZConjugateHandle Conjugate(void* unmanaged) const = 0;
		virtual void* Conjugate(FZConjugateHandle managed) const = 0;
	};
}


