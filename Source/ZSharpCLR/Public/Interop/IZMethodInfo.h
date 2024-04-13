// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"

namespace ZSharp
{
	struct FZInteropBuffer;
	class IZConjugate;

	class ZSHARPCLR_API IZMethodInfo : public IZMemberInfo
	{
	public:
		virtual int32 Invoke(const IZMethodInfo* method, IZConjugate* context, FZInteropBuffer* buffer) const = 0;
	};
}
