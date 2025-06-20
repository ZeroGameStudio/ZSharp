// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZStructPropertyVisitor final : public FZPropertyVisitorBase
	{

	public:
		explicit FZStructPropertyVisitor(const FStructProperty* underlyingProperty);

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override;
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override;
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override;

	private:
		const FStructProperty* UnderlyingStructProperty;
		bool bForceGetValue; // Conjugate registry can't deal with the first struct member variable.
		
	};
}


