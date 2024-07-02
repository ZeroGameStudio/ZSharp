// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZObjectPropertyVisitor : public FZPropertyVisitorBase
	{
	
	public:
		FZObjectPropertyVisitor(const FObjectProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingObjectProperty(underlyingProperty){}

	public:
		virtual void GetValue_InContainer(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FObjectProperty* UnderlyingObjectProperty;
		
	};
}


