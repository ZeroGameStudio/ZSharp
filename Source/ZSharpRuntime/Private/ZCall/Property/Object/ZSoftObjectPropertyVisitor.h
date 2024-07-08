// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectPropertyVisitorBase.h"

namespace ZSharp
{
	class FZSoftObjectPropertyVisitor final : public FZObjectPropertyVisitorBase
	{
	
	public:
		explicit FZSoftObjectPropertyVisitor(const FSoftObjectProperty* underlyingProperty)
			: FZObjectPropertyVisitorBase(underlyingProperty)
			, UnderlyingObjectProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FSoftObjectProperty* UnderlyingObjectProperty;
		
	};
}


