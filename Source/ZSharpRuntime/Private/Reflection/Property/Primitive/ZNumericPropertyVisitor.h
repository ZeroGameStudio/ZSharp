// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZPrimitivePropertyVisitorBase.h"

namespace ZSharp
{
	class FZNumericPropertyVisitor : public FZPrimitivePropertyVisitorBase
	{

	public:
		explicit FZNumericPropertyVisitor(const FNumericProperty* underlyingProperty)
			: FZPrimitivePropertyVisitorBase(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;
		
	};
}


