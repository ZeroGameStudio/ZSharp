// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZPrimitivePropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZPrimitivePropertyVisitor(const FProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty){}

	public:
		virtual bool IsPrimitive() const override { return true; }

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override { GetValue(src, dest); }
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;
		
	};
}


