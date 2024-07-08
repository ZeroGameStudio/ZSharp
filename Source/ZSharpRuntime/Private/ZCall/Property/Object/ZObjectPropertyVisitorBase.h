// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZObjectPropertyVisitorBase : public FZPropertyVisitorBase
	{
	
	public:
		explicit FZObjectPropertyVisitorBase(const FProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override final { GetRef(src, dest); }
		
	};
}


