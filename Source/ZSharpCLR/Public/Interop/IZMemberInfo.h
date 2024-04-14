// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZGCHandle.h"

namespace ZSharp
{
	class ZSHARPCLR_API IZMemberInfo : public IZGCHandle
	{
	public:
		virtual const FString& GetName() const = 0;
	};
}
