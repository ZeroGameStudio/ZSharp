// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/ZPropertyVisitorBase.h"

namespace ZSharp
{
	class FZObjectPropertyVisitorBase : public FZPropertyVisitorBase
	{
	
	public:
		explicit FZObjectPropertyVisitorBase(const FProperty* underlyingProperty)
			: FZPropertyVisitorBase(underlyingProperty){}
		
	};
}


