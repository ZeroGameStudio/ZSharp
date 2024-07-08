// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZObjectPropertyVisitorBase : public FZPropertyVisitorBase
	{
	
	public:
		explicit FZObjectPropertyVisitorBase(const FProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty){}

	public:
		virtual bool IsValueSemantics() const override final { return false; }

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override final { GetRef(src, dest); }
		
	};
}


