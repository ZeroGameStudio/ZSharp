// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZPrimitivePropertyVisitorBase : public FZPropertyVisitorBase
	{

	public:
		explicit FZPrimitivePropertyVisitorBase(const FProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty){}

	public:
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override final { GetValue(src, dest); }
		
	};
}


