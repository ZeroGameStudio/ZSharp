// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"

namespace ZSharp
{
	struct FZCallBuffer;
	class IZConjugate;

	class ZSHARPCLR_API IZPropertyInfo : public IZMemberInfo
	{
	public:
		virtual int32 Get(FZCallBuffer* buffer) const = 0;
		virtual int32 Set(FZCallBuffer* buffer) const = 0;
	};
}
