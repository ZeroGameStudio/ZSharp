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
		FZMasterAssemblyLoadContext(FZGCHandle handle, TUniqueFunction<void()>&& unloadCallback);
		virtual ~FZMasterAssemblyLoadContext() override;

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZAssemblyLoadContext
		virtual void Unload() override;

		virtual EZLoadAssemblyErrorCode LoadAssembly(const FString& assemblyName, void* args) override;
		virtual EZInvokeMethodErrorCode InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args) override;

		// IZMasterAssemblyLoadContext
		virtual FZRuntimeTypeHandle GetType(const FZRuntimeTypeLocatorWrapper& locator) override;

		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) override;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) override;

		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;
		virtual EZCallErrorCode ZCall(FZCallHandle handle, FZCallBuffer* buffer) override;
		virtual FZCallHandle GetZCallHandle(const FString& name) override;
		virtual void* BuildConjugate(void* unmanaged, FZRuntimeTypeHandle type) override;
		virtual void ReleaseConjugate(void* unmanaged) override;

		virtual IZConjugateRegistry& GetConjugateRegistry(uint16 id) override;
		
	public:
		EZCallErrorCode ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer) const;
		FZCallHandle GetZCallHandle_Black(const FString& name);
		void* BuildConjugate_Black(uint16 registryId, void* userdata);
		void ReleaseConjugate_Black(uint16 registryId, void* unmanaged);

	private:
		bool Tick(float deltaTime);
		
		EZCallErrorCode ZCall_Red(FZCallHandle handle, FZCallBuffer* buffer);
		FZCallHandle GetZCallHandle_Red(const FString& name);
		void* BuildConjugate_Red(void* unmanaged, FZRuntimeTypeHandle type);
		void ReleaseConjugate_Red(void* unmanaged);

		bool IsInRedStack() const { return !!RedStackDepth; }
		
	private:
		void HandleGarbageCollectComplete();

	private:
		FZGCHandle Handle;
		TUniqueFunction<void()> UnloadCallback;
		bool bUnloaded;

		TMap<FZCallHandle, TUniquePtr<IZCallDispatcher>> ZCallMap;
		TMap<FString, FZCallHandle> ZCallName2Handle;

		TArray<TTuple<uint64, TUniquePtr<IZCallResolver>>> ZCallResolverLink;

		TSparseArray<TUniquePtr<IZConjugateRegistry>> ConjugateRegistries;
		int32 RedStackDepth;

		FTSTicker::FDelegateHandle TickDelegate;
		FDelegateHandle GCDelegate;
		
	};
}


