// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectPropertyVisitorBase.h"

namespace ZSharp
{
	class FZWeakObjectPropertyVisitor final : public FZObjectPropertyVisitorBase
	{
	
	public:
		explicit FZWeakObjectPropertyVisitor(const FWeakObjectProperty* underlyingProperty)
			: FZObjectPropertyVisitorBase(underlyingProperty)
			, UnderlyingObjectProperty(underlyingProperty){}

	public:
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FWeakObjectProperty* UnderlyingObjectProperty;
		
	};
}


