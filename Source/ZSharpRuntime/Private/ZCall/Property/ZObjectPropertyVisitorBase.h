﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZObjectPropertyVisitorBase : public FZPropertyVisitorBase
	{
	
	public:
		FZObjectPropertyVisitorBase(const FProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty){}

	public:
		virtual bool IsValueSemantics() const override { return false; }

	public:
		virtual void GetValue_InContainer(const void* src, FZCallBufferSlot& dest) const override { GetRef_InContainer(src, dest); }
		
	};
}

