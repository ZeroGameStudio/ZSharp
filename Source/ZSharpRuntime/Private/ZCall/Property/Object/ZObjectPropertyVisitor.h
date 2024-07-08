// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectPropertyVisitorBase.h"

namespace ZSharp
{
	class FZObjectPropertyVisitor final : public FZObjectPropertyVisitorBase
	{
	
	public:
		explicit FZObjectPropertyVisitor(const FObjectProperty* underlyingProperty)
			: FZObjectPropertyVisitorBase(underlyingProperty)
			, UnderlyingObjectProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override { GetRef(src, dest); }
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FObjectProperty* UnderlyingObjectProperty;
		
	};
}


