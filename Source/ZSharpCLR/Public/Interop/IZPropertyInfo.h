// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"

namespace ZSharp
{
	struct FZInteropBuffer;
	class IZConjugate;

	class ZSHARPCLR_API IZPropertyInfo : public IZMemberInfo
	{
	public:
		virtual int32 Get(const IZPropertyInfo* property, IZConjugate* context, FZInteropBuffer* buffer) = 0;
		virtual int32 Set(const IZPropertyInfo* property, IZConjugate* context, FZInteropBuffer* buffer) = 0;
	};
}
