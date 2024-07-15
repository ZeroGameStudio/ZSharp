// Copyright Zero Games. All Rights Reserved.


#include "ZFunctionVisitorHandle.h"

#include "ZFunctionVisitorRegistry.h"

const ZSharp::FZFunctionVisitor* ZSharp::FZFunctionVisitorHandle::Get() const
{
	return FZFunctionVisitorRegistry::Get().Get(*this);
}


