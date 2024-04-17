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

	public:
		FZMasterAssemblyLoadContext(FZGCHandle handle, const TFunction<void()>& unloadCallback)
			: Handle(handle)
			, UnloadCallback(unloadCallback){}
		
	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override;

		// IZAssemblyLoadContext
		virtual void Unload() override;

		// IZMasterAssemblyLoadContext
		virtual IZAssembly* LoadAssembly(const TArray<uint8>& buffer, void* args = nullptr) override;
		virtual IZAssembly* GetAssembly(const FString& name) const override;

		virtual IZType* GetType(const FString& name) const override;

		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) override;
		virtual IZCallDispatcher* GetZCallDispatcher(FZCallHandle handle) const override;
		virtual IZCallDispatcher* GetZCallDispatcher(const FString& name) const override;
		virtual IZCallDispatcher* GetOrResolveZCallDispatcher(const FString& name) override;
		virtual FZCallHandle GetZCallHandle(const IZCallDispatcher* dispatcher) const override;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) override;

		virtual FZConjugateHandle BuildConjugate(void* unmanaged, const IZType* managedType) override;
		virtual void BuildConjugate(void* unmanaged, FZConjugateHandle managed) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void ReleaseConjugate(FZConjugateHandle managed) override;
		virtual FZConjugateHandle Conjugate(void* unmanaged) const override;
		virtual void* Conjugate(FZConjugateHandle managed) const override;

	private:
		FZGCHandle Handle;
		TFunction<void()> UnloadCallback;

		TMap<FString, TUniquePtr<IZAssembly>> AssemblyMap;

		TMap<FZCallHandle, TUniquePtr<IZCallDispatcher>> ZCallMap;
		TMap<FString, IZCallDispatcher*> Name2ZCall;
		TMap<IZCallDispatcher*, FZCallHandle> ZCall2Handle;

		TArray<TTuple<uint64, TUniquePtr<IZCallResolver>>> ZCallResolverLink;

		TMap<void*, FZConjugateHandle> ConjugateUnmanaged2Managed;
		TMap<FZConjugateHandle, void*> ConjugateManaged2Unmanaged;
		
	};
}
