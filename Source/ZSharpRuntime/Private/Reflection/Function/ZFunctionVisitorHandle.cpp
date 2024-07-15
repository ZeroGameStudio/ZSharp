// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Function/ZFunctionVisitorHandle.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"

const ZSharp::FZFunctionVisitor* ZSharp::FZFunctionVisitorHandle::Get() const
{
	return FZFunctionVisitorRegistry::Get().Get(*this);
}


