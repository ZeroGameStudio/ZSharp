// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"
#include "ZConjugateHandle.h"

namespace ZSharp
{
	class IZMethodInfo;
	class IZPropertyInfo;
	struct FZCallBuffer;
	
	class ZSHARPCLR_API IZType : public IZMemberInfo
	{
	public:
		virtual int32 New(FZCallBuffer* buffer) const = 0;
		virtual FZConjugateHandle BuildConjugate(void* unmanaged) const = 0;
	public:
		virtual const IZMethodInfo* GetMethod(const FString& name) const = 0;
		virtual const IZPropertyInfo* GetProperty(const FString& name) const = 0;
	};
}
