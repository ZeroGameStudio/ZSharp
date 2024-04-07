// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"

namespace ZSharp
{
	class ZSHARPCLR_API IZMethodInfo : public IZMemberInfo
	{
	public:
		virtual void Invoke(void* buffer) const = 0;
	};
}
