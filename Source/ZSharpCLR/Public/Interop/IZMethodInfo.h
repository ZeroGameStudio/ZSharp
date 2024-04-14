// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"

namespace ZSharp
{
	struct FZCallBuffer;

	class ZSHARPCLR_API IZMethodInfo : public IZMemberInfo
	{
	public:
		virtual bool IsZCallable() const = 0;
	public:
		virtual int32 Invoke(FZCallBuffer* buffer) const = 0;
	};
}
