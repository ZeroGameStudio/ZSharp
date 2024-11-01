// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZMasterAssemblyLoadContext.h"

#include "Interop/ZGCHandle.h"
#include "ZCall/IZCallDispatcher.h"
#include "ZCall/IZCallResolver.h"
#include "Conjugate/IZConjugateRegistry.h"

namespace ZSharp
{
	class FZMasterAssemblyLoadContext final : public IZMasterAssemblyLoadContext
	{

		using ThisClass = FZMasterAssemblyLoadContext;

	public:
		FZMasterAssemblyLoadContext(TUniqueFunction<void()>&& unloadCallback);
		virtual ~FZMasterAssemblyLoadContext() override;

	public:
		// IZAssemblyLoadContext
		virtual void Unload() override;

		virtual EZLoadAssemblyErrorCode LoadAssembly(const FString& assemblyName, void* args) override;
		virtual EZInvokeMethodErrorCode InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args) override;

		// IZMasterAssemblyLoadContext
		virtual FZRuntimeTypeHandle GetType(const FZRuntimeTypeUri& uri) override;

		virtual IZConjugateRegistry& GetConjugateRegistry(uint16 id) const override;
		virtual void* BuildConjugate(void* unmanaged, FZRuntimeTypeHandle type) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;
		
		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) override;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) override;
		virtual FZCallHandle GetZCallHandle(const FString& name) override;
		virtual EZCallErrorCode ZCall(FZCallHandle handle, FZCallBuffer* buffer) override;
		
	public:
		void* BuildConjugate_Black(uint16 registryId, void* userdata);
		void ReleaseConjugate_Black(uint16 registryId, void* unmanaged);
		
		FZCallHandle GetZCallHandle_Black(const FString& name);
		EZCallErrorCode ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer) const;
	
	private:
		void* BuildConjugate_Red(void* unmanaged, FZRuntimeTypeHandle type);
		void ReleaseConjugate_Red(void* unmanaged);
		
		FZCallHandle GetZCallHandle_Red(const FString& name);
		EZCallErrorCode ZCall_Red(FZCallHandle handle, FZCallBuffer* buffer);

	private:
		bool IsInRedStack() const { return !!RedStackDepth; }
		
	private:
		void HandleGarbageCollectComplete();

	private:
		TUniqueFunction<void()> UnloadCallback;
		bool bUnloaded;

		TMap<FZCallHandle, TUniquePtr<IZCallDispatcher>> ZCallMap;
		TMap<FString, FZCallHandle> ZCallName2Handle;

		TArray<TTuple<uint64, TUniquePtr<IZCallResolver>>> ZCallResolverLink;

		TSparseArray<TUniquePtr<IZConjugateRegistry>> ConjugateRegistries;
		int32 RedStackDepth;

		FDelegateHandle GCDelegate;
		
	};
}


