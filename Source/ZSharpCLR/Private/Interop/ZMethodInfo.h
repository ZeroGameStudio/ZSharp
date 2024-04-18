// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZMethodInfo.h"

namespace ZSharp
{
	class FZMethodInfo : public IZMethodInfo
	{

	public:
		FZMethodInfo(FZGCHandle handle)
			:Handle(handle){}

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZMemberInfo
		virtual const FString& GetName() const override;
		virtual bool IsZCallable() const override;

		// IZMethodInfo
		virtual int32 Invoke(FZCallBuffer* buffer) const override;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;
		
	};
}


