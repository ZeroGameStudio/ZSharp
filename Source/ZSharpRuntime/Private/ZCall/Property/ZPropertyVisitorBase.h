// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZPropertyVisitor.h"
#include "UObject/UnrealType.h"

namespace ZSharp
{
	struct FZCallBufferSlot;

	class FZPropertyVisitorBase : public IZPropertyVisitor
	{

	public:
		explicit FZPropertyVisitorBase(const FProperty* underlyingProperty)
			: UnderlyingProperty(underlyingProperty){}

	public:
		virtual bool IsPrimitive() const override { return false; }
		virtual bool IsValueSemantics() const override { return true; }
		
	public:
		virtual void InitializeValue_InContainer(void* dest) const override;

	protected:
		const FProperty* UnderlyingProperty;
		
	};
}


