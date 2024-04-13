// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZMasterAssemblyLoadContext.h"

#include "Interop/IZAssembly.h"

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

	private:
		FZGCHandle Handle;
		TFunction<void()> UnloadCallback;

		TMap<FString, TUniquePtr<IZAssembly>> AssemblyMap;
		
	};
}
