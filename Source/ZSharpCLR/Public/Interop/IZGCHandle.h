﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZGCHandle.h"

namespace ZSharp
{
	class ZSHARPCLR_API IZGCHandle : public FNoncopyable
	{
	public:
		virtual ~IZGCHandle(){}
	public:
		virtual FZGCHandle GetGCHandle() const = 0;
	};
}

