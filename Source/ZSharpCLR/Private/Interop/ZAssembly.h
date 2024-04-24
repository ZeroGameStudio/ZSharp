// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZAssembly.h"
#include "Interop/IZType.h"

namespace ZSharp
{
	class FZAssembly : public IZAssembly
	{

	public:
		FZAssembly(FZGCHandle handle)
			: Handle(handle){}
		
		virtual ~FZAssembly() override { Free(Handle); }

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZAssembly
		virtual const FString& GetName() const override;
		
		virtual const IZType* GetType(const FString& name) const override;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;
		
		mutable TMap<FString, TUniquePtr<IZType>> TypeMap;
		
	};
}
