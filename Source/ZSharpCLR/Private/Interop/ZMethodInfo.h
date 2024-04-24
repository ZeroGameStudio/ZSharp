// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZMethodInfo.h"

namespace ZSharp
{
	class FZMethodInfo : public IZMethodInfo
	{

		using ThisClass = FZMethodInfo;

	public:
		FZMethodInfo(FZGCHandle handle)
			: Handle(handle){}

		virtual ~FZMethodInfo() override { Free(Handle); }

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZMemberInfo
		virtual const FString& GetName() const override;
		virtual bool IsZCallable() const override;

		// IZMethodInfo
		virtual ZCALL_TO_MANAGED_AUTO_BUFFER(Invoke) override;
		virtual ZCALL_TO_MANAGED_USER_BUFFER(Invoke) override;
		virtual ZCALL_TO_MANAGED_USER_BUFFER_CHECKED(Invoke) override;

	private:
		int32 GetNumSlots() const;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;
		mutable TOptional<int32> CachedNumSlots;
		
	};
}


