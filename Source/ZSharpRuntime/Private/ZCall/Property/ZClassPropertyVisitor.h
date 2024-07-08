// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectPropertyVisitorBase.h"

namespace ZSharp
{
	class FZClassPropertyVisitor final : public FZObjectPropertyVisitorBase
	{
	
	public:
		explicit FZClassPropertyVisitor(const FClassProperty* underlyingProperty)
			: FZObjectPropertyVisitorBase(underlyingProperty)
			, UnderlyingClassProperty(underlyingProperty){}

	public:
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FClassProperty* UnderlyingClassProperty;
		
	};
}


