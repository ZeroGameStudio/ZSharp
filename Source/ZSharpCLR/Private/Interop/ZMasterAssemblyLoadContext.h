// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZMasterAssemblyLoadContext.h"

namespace ZSharp
{
	class FZMasterAssemblyLoadContext : public IZMasterAssemblyLoadContext
	{

	public:
		FZMasterAssemblyLoadContext(FZGCHandle handle)
			: Handle(handle){}
		
	public:
		// IZConjugate
		virtual void Release() override;
		virtual FZGCHandle GetGCHandle() const override;

		// IZAssemblyLoadContext
		virtual void Unload() override;

		// IZMasterAssemblyLoadContext
		virtual IZAssembly* LoadAssembly(const FString& name, TArray<uint8> buffer) override;
		virtual IZAssembly* GetAssembly(const FString& name) const override;

	private:
		FZGCHandle Handle;
		
	};
}
