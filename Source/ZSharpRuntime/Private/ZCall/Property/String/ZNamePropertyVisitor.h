// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZNamePropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZNamePropertyVisitor(const FNameProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingNameProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FNameProperty* UnderlyingNameProperty;
		
	};
}


