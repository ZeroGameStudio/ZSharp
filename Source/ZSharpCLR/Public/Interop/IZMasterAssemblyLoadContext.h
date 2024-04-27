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
	struct FZCallBuffer;

	DECLARE_MULTICAST_DELEGATE(FZPreZCallToManaged)
	DECLARE_MULTICAST_DELEGATE(FZPostZCallToManaged)
	
	class ZSHARPCLR_API IZMasterAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual const IZAssembly* LoadAssembly(const TArray<uint8>& buffer, void* args = nullptr) = 0;
		virtual const IZAssembly* GetAssembly(const FString& name) const = 0;
	public:
		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) = 0;
		virtual int32 ZCall(FZCallHandle handle, FZCallBuffer* buffer) const = 0;
		virtual int32 ZCall(const FString& name, FZCallBuffer* buffer) const = 0;
		virtual int32 ZCall(const FString& name, FZCallBuffer* buffer, FZCallHandle* outHandle = nullptr) = 0;
		virtual void ZCall_AnyThread(FZCallHandle handle, FZCallBuffer* buffer, int32 numSlots) = 0;
		virtual FZCallHandle GetZCallHandle(const FString& name) const = 0;
		virtual FZCallHandle GetOrResolveZCallHandle(const FString& name) = 0;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) = 0;

		virtual FDelegateHandle RegisterPreZCallToManaged(FZPreZCallToManaged::FDelegate delegate) = 0;
		virtual void UnregisterPreZCallToManaged(FDelegateHandle delegate) = 0;
		virtual void UnregisterPreZCallToManaged(const void* userObject) = 0;
		virtual FDelegateHandle RegisterPostZCallToManaged(FZPostZCallToManaged::FDelegate delegate) = 0;
		virtual void UnregisterPostZCallToManaged(FDelegateHandle delegate) = 0;
		virtual void UnregisterPostZCallToManaged(const void* userObject) = 0;
	public:
		virtual FZConjugateHandle BuildConjugate(void* unmanaged, const IZType* managedType) = 0;
		virtual void BuildConjugate(void* unmanaged, FZConjugateHandle managed) = 0;
		virtual void ReleaseConjugate(void* unmanaged) = 0;
		virtual void ReleaseConjugate(FZConjugateHandle managed) = 0;
		virtual FZConjugateHandle Conjugate(void* unmanaged) const = 0;
		virtual void* Conjugate(FZConjugateHandle managed) const = 0;
		// Helpers
	public:
		const IZAssembly* LoadAssembly(const FString& path, void* args = nullptr);
	};
}


