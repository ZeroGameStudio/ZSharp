﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZGCHandle.h"

namespace ZSharp
{
	class ZSHARPCORE_API IZMemberInfo : public IZGCHandle
	{
	public:
		virtual const FString& GetName() const = 0;
	public:
		virtual bool IsZCallable() const = 0;
	};
}