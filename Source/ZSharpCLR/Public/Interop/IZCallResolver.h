﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZCallDispatcher;
	
	class ZSHARPCLR_API IZCallResolver
	{
	public:
		virtual ~IZCallResolver(){}
	public:
		virtual IZCallDispatcher* Resolve(const FString& name) const = 0;
	};
}