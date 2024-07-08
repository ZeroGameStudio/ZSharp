// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZNumericPropertyVisitor.h"
#include "ZPrimitivePropertyVisitorBase.h"

namespace ZSharp
{
	class FZEnumPropertyVisitor : public FZPrimitivePropertyVisitorBase
	{

	public:
		explicit FZEnumPropertyVisitor(const FEnumProperty* underlyingProperty)
			: FZPrimitivePropertyVisitorBase(underlyingProperty)
			, UnderlyingNumericProperty(MakeUnique<FZNumericPropertyVisitor>(underlyingProperty->GetUnderlyingProperty())){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		TUniquePtr<FZNumericPropertyVisitor> UnderlyingNumericProperty;
		
	};
}


