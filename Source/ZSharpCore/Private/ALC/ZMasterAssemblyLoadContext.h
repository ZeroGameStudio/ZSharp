// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZMasterAssemblyLoadContext.h"

#include "Interop/ZGCHandle.h"
#include "ZCall/IZCallDispatcher.h"
#include "ZCall/IZCallResolver.h"
#include "ZCall/IZConjugateRegistry.h"

namespace ZSharp
{
	class FZMasterAssemblyLoadContext : public IZMasterAssemblyLoadContext
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

		// IZMasterAssemblyLoadContext
		virtual void LoadAssembly(const TArray<uint8>& buffer, void* args = nullptr) override;
		virtual FZRuntimeTypeHandle GetType(const FString& assemblyName, const FString& typeName) override;

		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) override;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) override;

		virtual int32 ZCall(FZCallHandle handle, FZCallBuffer* buffer) override;
		virtual FZCallHandle GetZCallHandle(const FString& name) override;
		virtual void* BuildConjugate(void* unmanaged, FZRuntimeTypeHandle type) override;
		virtual void ReleaseConjugate(void* unmanaged) override;

		virtual IZConjugateRegistry& GetConjugateRegistry(uint16 id) override;
		
	public:
		int32 ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer) const;
		FZCallHandle GetZCallHandle_Black(const FString& name);
		void* BuildConjugate_Black(uint16 registryId);
		void ReleaseConjugate_Black(uint16 registryId, void* unmanaged);

	private:
		bool Tick(float deltaTime);
		
		int32 ZCall_Red(FZCallHandle handle, FZCallBuffer* buffer);
		FZCallHandle GetZCallHandle_Red(const FString& name);
		void* BuildConjugate_Red(void* unmanaged, FZRuntimeTypeHandle type);
		void ReleaseConjugate_Red(void* unmanaged);

		void PushRedFrame();
		void PopRedFrame();

		bool IsInsideOfRedZCall() const { return !!RedZCallDepth; }

	private:
		FZGCHandle Handle;
		TUniqueFunction<void()> UnloadCallback;

		TMap<FZCallHandle, TUniquePtr<IZCallDispatcher>> ZCallMap;
		TMap<FString, FZCallHandle> ZCallName2Handle;

		TArray<TTuple<uint64, TUniquePtr<IZCallResolver>>> ZCallResolverLink;

		TSparseArray<TUniquePtr<IZConjugateRegistry>> ConjugateRegistries;
		int32 RedZCallDepth;

		
		
	};
}
