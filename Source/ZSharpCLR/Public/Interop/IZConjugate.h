// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZGCHandle.h"

namespace ZSharp
{
	class ZSHARPCLR_API IZConjugate : public FNoncopyable
	{
	public:
		virtual ~IZConjugate(){}
	public:
		virtual void Release() = 0;
		virtual FZGCHandle GetGCHandle() const = 0;
	};
}


