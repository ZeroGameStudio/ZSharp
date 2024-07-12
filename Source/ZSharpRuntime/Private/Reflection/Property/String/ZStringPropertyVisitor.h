// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZStringPropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZStringPropertyVisitor(const FStrProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingStringProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FStrProperty* UnderlyingStringProperty;
		
	};
}


