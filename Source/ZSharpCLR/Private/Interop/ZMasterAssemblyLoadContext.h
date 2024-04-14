// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZMasterAssemblyLoadContext.h"

#include "Interop/IZAssembly.h"
#include "Interop/IZCallDispatcher.h"

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
		virtual IZAssembly* LoadAssembly(const TArray<uint8>& buffer) override;
		virtual IZAssembly* GetAssembly(const FString& name) const override;

		virtual IZType* GetType(const FString& name) const override;

		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) override;
		virtual IZCallDispatcher* GetZCallDispatcher(FZCallHandle handle) const override;
		virtual IZCallDispatcher* GetZCallDispatcher(const FString& name) const override;
		virtual FZCallHandle GetZCallHandle(const IZCallDispatcher* dispatcher) const override;

		virtual FZGCHandle BuildConjugate(void* native, const IZType* type) override;
		virtual void BuildConjugate(void* native, FZGCHandle handle) override;
		virtual void ReleaseConjugate(void* native) override;
		virtual void ReleaseConjugate(FZGCHandle handle) override;

	private:
		FZGCHandle Handle;
		TFunction<void()> UnloadCallback;

		TMap<FString, TUniquePtr<IZAssembly>> AssemblyMap;

		TMap<FZCallHandle, TUniquePtr<IZCallDispatcher>> ZCallMap;
		TMap<FString, IZCallDispatcher*> Name2ZCall;
		TMap<IZCallDispatcher*, FZCallHandle> ZCall2Handle;

		TMap<void*, FZGCHandle> Native2Conjugate;
		TMap<FZGCHandle, void*> Conjugate2Native;
		
	};
}
