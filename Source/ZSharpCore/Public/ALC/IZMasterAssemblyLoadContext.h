// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "ZCall/ZCallHandle.h"
#include "ZCall/CZConjugateRegistryImpl.h"

namespace ZSharp
{
	class IZCallDispatcher;
	class IZCallResolver;
	struct FZCallBuffer;
	class IZConjugateRegistry;
	
	class ZSHARPCORE_API IZMasterAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual void LoadAssembly(const TArray<uint8>& buffer, void* args = nullptr) = 0;
		virtual FZRuntimeTypeHandle GetType(const FString& assemblyName, const FString& typeName) = 0;
	public:
		virtual FZCallHandle RegisterZCall(IZCallDispatcher* dispatcher) = 0;
		virtual void RegisterZCallResolver(IZCallResolver* resolver, uint64 priority) = 0;

		virtual int32 ZCall(FZCallHandle handle, FZCallBuffer* buffer) = 0;
		virtual FZCallHandle GetZCallHandle(const FString& name) = 0;
		virtual void* BuildConjugate(void* unmanaged, FZRuntimeTypeHandle type) = 0;
		virtual void ReleaseConjugate(void* unmanaged) = 0;
		
		virtual void VisitConjugateRegistry(uint16 id, const TFunctionRef<void(IZConjugateRegistry&)> action) = 0;
		// Helpers
	public:
		void LoadAssembly(const FString& path, void* args = nullptr);

		template <CZConjugateRegistryImpl T>
		void VisitConjugateRegistry(const TFunctionRef<void(T&)> action)
		{
			VisitConjugateRegistry(T::RegistryId, [action](IZConjugateRegistry& registry){ action(StaticCast<T&>(registry)); });
		}
	};
}


