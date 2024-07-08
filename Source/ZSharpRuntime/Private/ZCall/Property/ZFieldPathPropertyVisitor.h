// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZFieldPathPropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZFieldPathPropertyVisitor(const FFieldPathProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingFieldPathProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FFieldPathProperty* UnderlyingFieldPathProperty;
		
	};
}


