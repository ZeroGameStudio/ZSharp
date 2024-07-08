// Copyright Zero Games. All Rights Reserved.

#pragma once
#include "ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZStructPropertyVisitor final : public FZPropertyVisitorBase
	{

	public:
		explicit FZStructPropertyVisitor(const FStructProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingStructProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FStructProperty* UnderlyingStructProperty;
		
	};
}


