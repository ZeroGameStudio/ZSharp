﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZPropertyInfo.h"

namespace ZSharp
{
	class FZPropertyInfo : public IZPropertyInfo
	{

	public:
		FZPropertyInfo(FZGCHandle handle)
			:Handle(handle){}

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZMemberInfo
		virtual const FString& GetName() const override;
		virtual bool IsZCallable() const override;

		// IZPropertyInfo
		virtual int32 Get(FZCallBuffer* buffer) const override;
		virtual int32 Set(FZCallBuffer* buffer) const override;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;
		
	};
}

