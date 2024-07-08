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
		virtual void InitializeValue(void* dest) const override;

	protected:
		virtual const FProperty* GetUnderlyingProperty() const override final { return UnderlyingProperty; }

	protected:
		const FProperty* UnderlyingProperty;
		
	};
}


