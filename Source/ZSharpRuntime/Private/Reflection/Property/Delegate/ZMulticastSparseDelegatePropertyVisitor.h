﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZMulticastSparseDelegatePropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZMulticastSparseDelegatePropertyVisitor(const FMulticastDelegateProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingDelegateProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FMulticastDelegateProperty* UnderlyingDelegateProperty;
		
	};
}

