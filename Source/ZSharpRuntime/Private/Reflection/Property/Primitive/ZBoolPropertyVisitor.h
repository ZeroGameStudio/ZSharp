// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZPrimitivePropertyVisitorBase.h"

namespace ZSharp
{
	class FZBoolPropertyVisitor : public FZPrimitivePropertyVisitorBase
	{

	public:
		explicit FZBoolPropertyVisitor(const FBoolProperty* underlyingProperty)
			: FZPrimitivePropertyVisitorBase(underlyingProperty)
			, UnderlyingBoolProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FBoolProperty* UnderlyingBoolProperty;
		
	};
}


