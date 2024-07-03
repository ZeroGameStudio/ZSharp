// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectPropertyVisitorBase.h"

namespace ZSharp
{
	class FZObjectPropertyVisitor : public FZObjectPropertyVisitorBase
	{
	
	public:
		explicit FZObjectPropertyVisitor(const FObjectProperty* underlyingProperty)
			: FZObjectPropertyVisitorBase(underlyingProperty)
			, UnderlyingObjectProperty(underlyingProperty){}

	public:
		virtual void GetRef_InContainer(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FObjectProperty* UnderlyingObjectProperty;
		
	};
}


