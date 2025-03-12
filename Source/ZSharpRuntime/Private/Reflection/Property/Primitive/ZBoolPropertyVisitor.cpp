// Copyright Zero Games. All Rights Reserved.

#include "ZBoolPropertyVisitor.h"

#include "ZCall/ZCallBuffer.h"

void ZSharp::FZBoolPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteBool(UnderlyingBoolProperty->GetPropertyValue(src));
}

void ZSharp::FZBoolPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	uint8 value = src.ReadBool() ? 0xFF & UnderlyingBoolProperty->GetFieldMask() : 0;
	UnderlyingProperty->CopySingleValue(dest, &value);
}


