// Copyright Zero Games. All Rights Reserved.

#pragma once
#include "ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZStructPropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZStructPropertyVisitor(const FStructProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingStructProperty(underlyingProperty){}

	public:
		virtual void GetValue_InContainer(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef_InContainer(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FStructProperty* UnderlyingStructProperty;
		
	};
}


