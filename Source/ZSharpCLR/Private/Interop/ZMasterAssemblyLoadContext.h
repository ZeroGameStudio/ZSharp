// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZMasterAssemblyLoadContext.h"

#include "Interop/IZAssembly.h"
#include "Interop/IZCallDispatcher.h"
#include "Interop/IZCallResolver.h"

namespace ZSharp
{
	class FZMasterAssemblyLoadContext : public IZMasterAssemblyLoadContext
	{

		using ThisClass = FZMasterAssemblyLoadContext;

	public:
		FZMasterAssemblyLoadContext(FZGCHandle handle, const TFunction<void()>& unloadCallback);
		virtual ~FZMasterAssemblyLoadContext() override;

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZAssemblyLoadContext
		virtual void Unload() override;

		// IZMasterAssemblyLoadContext
		virtual const IZAssembly* LoadAssembly(const TArray<uint8>& buffer, void* args = nullptr) override;
		virtual const IZAssembly* GetAssembly(const FString& name) const override;

		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) override;
		virtual int32 ZCall(FZCallHandle handle, FZCallBuffer* buffer) const override;
		virtual int32 ZCall(const FString& name, FZCallBuffer* buffer) const override;
		virtual int32 ZCall(const FString& name, FZCallBuffer* buffer, FZCallHandle* outHandle = nullptr) override;
		virtual void ZCall_AnyThread(FZCallHandle handle, FZCallBuffer* buffer, int32 numSlots) override;
		virtual FZCallHandle GetZCallHandle(const FString& name) const override;
		virtual FZCallHandle GetOrResolveZCallHandle(const FString& name) override;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) override;

		virtual FDelegateHandle RegisterPreZCallToManaged(FZPreZCallToManaged::FDelegate delegate) override;
		virtual void UnregisterPreZCallToManaged(FDelegateHandle delegate) override;
		virtual void UnregisterPreZCallToManaged(const void* userObject) override;
		virtual FDelegateHandle RegisterPostZCallToManaged(FZPostZCallToManaged::FDelegate delegate) override;
		virtual void UnregisterPostZCallToManaged(FDelegateHandle delegate) override;
		virtual void UnregisterPostZCallToManaged(const void* userObject) override;

		virtual FZConjugateHandle BuildConjugate(void* unmanaged, const IZType* managedType) override;
		virtual void BuildConjugate(void* unmanaged, FZConjugateHandle managed) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void ReleaseConjugate(FZConjugateHandle managed) override;
		virtual FZConjugateHandle Conjugate(void* unmanaged) const override;
		virtual void* Conjugate(FZConjugateHandle managed) const override;

	public:
		void NotifyPreZCallToManaged() const;
		void NotifyPostZCallToManaged() const;

	private:
		bool Tick(float deltaTime);
		void FlushDeferredZCalls();

	private:
		FZGCHandle Handle;
		TFunction<void()> UnloadCallback;

		TMap<FString, TUniquePtr<IZAssembly>> AssemblyMap;

		TMap<FZCallHandle, TUniquePtr<IZCallDispatcher>> ZCallMap;
		TMap<FString, FZCallHandle> ZCallName2Handle;

		TArray<TTuple<uint64, TUniquePtr<IZCallResolver>>> ZCallResolverLink;

		FRWLock DeferredZCallsLock;
		TQueue<TTuple<FZCallHandle, FZCallBuffer>> DeferredZCalls;
		FTSTicker::FDelegateHandle TickerHandle;

		TMap<void*, FZConjugateHandle> ConjugateUnmanaged2Managed;
		TMap<FZConjugateHandle, void*> ConjugateManaged2Unmanaged;

		FZPreZCallToManaged PreZCallToManaged;
		FZPostZCallToManaged PostZCallToManaged;
		
	};
}
