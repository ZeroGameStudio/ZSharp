// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/ZPropertyVisitorBase.h"
#include "UObject/PropertyOptional.h"

namespace ZSharp
{
	class FZOptionalPropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZOptionalPropertyVisitor(const FOptionalProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingOptionalProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FOptionalProperty* UnderlyingOptionalProperty;
		
	};
}


