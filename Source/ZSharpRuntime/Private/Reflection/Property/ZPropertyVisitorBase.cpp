// Copyright Zero Games. All Rights Reserved.


#include "ZPropertyVisitorBase.h"

void ZSharp::FZPropertyVisitorBase::InitializeValue(void* dest) const
{
	UnderlyingProperty->InitializeValue(dest);
}

void ZSharp::FZPropertyVisitorBase::DestructValue(void* dest) const
{
	UnderlyingProperty->DestroyValue(dest);
}


