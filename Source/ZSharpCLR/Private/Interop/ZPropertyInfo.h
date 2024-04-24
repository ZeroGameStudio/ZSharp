// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZPropertyInfo.h"

namespace ZSharp
{
	class FZPropertyInfo : public IZPropertyInfo
	{

	public:
		FZPropertyInfo(FZGCHandle handle)
			:Handle(handle){}

		virtual ~FZPropertyInfo() override { Free(Handle); }

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZMemberInfo
		virtual const FString& GetName() const override;
		virtual bool IsZCallable() const override;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;
		
	};
}


