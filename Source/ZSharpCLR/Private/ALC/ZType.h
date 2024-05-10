// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZType.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	class FZType : public IZType
	{

	public:
		FZType(FZGCHandle handle)
			:Handle(handle){}

		virtual ~FZType() override { Handle.Free(); }

	public:
		// IZGCHandle
		virtual FZGCHandle GetGCHandle() const override { return Handle; }

		// IZMemberInfo
		virtual const FString& GetName() const override;
		virtual bool IsZCallable() const override;

		// IZType
		virtual FZConjugateHandle BuildConjugate(void* unmanaged) const override;

	private:
		FZGCHandle Handle;
		mutable TOptional<FString> CachedName;
		
	};
}


