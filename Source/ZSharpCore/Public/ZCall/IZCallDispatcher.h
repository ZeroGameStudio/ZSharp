// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	class ZSHARPCORE_API IZCallDispatcher
	{
	public:
		virtual ~IZCallDispatcher(){}
	public:
		virtual const FString& GetName() const = 0;
		virtual EZCallErrorCode Dispatch(FZCallBuffer* buffer) const = 0;
	};
}


