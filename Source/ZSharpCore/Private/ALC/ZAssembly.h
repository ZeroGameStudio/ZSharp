// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZAssembly.h"
#include "ALC/IZType.h"

namespace ZSharp
{
	class FZAssembly : public IZAssembly
	{

	public:
		FZAssembly(FZGCHandle handle)
			: Handle(handle){}
		
		virtual ~FZAssembly() override { Handle.Free(); }

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
