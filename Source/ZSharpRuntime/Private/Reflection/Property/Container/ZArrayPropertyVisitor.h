﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZArrayPropertyVisitor : public FZPropertyVisitorBase
	{

	public:
		explicit FZArrayPropertyVisitor(const FArrayProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty)
			, UnderlyingArrayProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FArrayProperty* UnderlyingArrayProperty;
		
	};
}


