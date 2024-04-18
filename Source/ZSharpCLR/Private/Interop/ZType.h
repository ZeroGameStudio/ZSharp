// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZType.h"
#include "Interop/ZConjugateHandle.h"
#include "Interop/IZMethodInfo.h"
#include "Interop/IZPropertyInfo.h"

namespace ZSharp
{
	class FZType : public IZType
	{

	public:
		FZType(FZGCHandle handle)
			:Handle(handle){}

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZMemberInfo
		virtual const FString& GetName() const override;
		virtual bool IsZCallable() const override;

		// IZType
		virtual int32 New(FZCallBuffer* buffer) const override;
		virtual FZConjugateHandle BuildConjugate(void* unmanaged) const override;

		virtual const IZMethodInfo* GetMethod(const FString& name) const override;
		virtual const IZPropertyInfo* GetProperty(const FString& name) const override;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;

		mutable TMap<FString, TUniquePtr<IZMethodInfo>> MethodMap;
		mutable TMap<FString, TUniquePtr<IZPropertyInfo>> PropertyMap;
		
	};
}


