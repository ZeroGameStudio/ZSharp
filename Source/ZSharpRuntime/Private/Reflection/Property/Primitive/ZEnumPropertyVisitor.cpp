// Copyright Zero Games. All Rights Reserved.


#include "ZEnumPropertyVisitor.h"

void ZSharp::FZEnumPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	UnderlyingNumericProperty->GetValue(src, dest);
}

void ZSharp::FZEnumPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	UnderlyingNumericProperty->SetValue(dest, src);
}


