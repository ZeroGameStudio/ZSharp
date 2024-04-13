// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZAssembly.h"

namespace ZSharp
{
	class FZAssembly : public IZAssembly
	{

	public:
		FZAssembly(FZGCHandle handle)
			: Handle(handle){}

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZAssembly
		virtual const FString& GetName() const override;

		virtual IZType* GetType(const FString& name) const override;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;
		
	};
}
