// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"

namespace ZSharp
{
	class IZMethodInfo;
	class IZPropertyInfo;
	
	class ZSHARPCLR_API IZType : public IZMemberInfo
	{
	public:
		virtual int32 New(FZCallBuffer* buffer) const = 0;
		virtual FZConjugateHandle BuildConjugate(void* unmanaged) const = 0;
	public:
		virtual IZMethodInfo* GetMethod(const FString& name) const = 0;
		virtual IZPropertyInfo* GetProperty(const FString& name) const = 0;
	};
}
