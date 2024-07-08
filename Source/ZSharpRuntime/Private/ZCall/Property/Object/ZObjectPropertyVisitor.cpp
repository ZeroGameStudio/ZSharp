// Copyright Zero Games. All Rights Reserved.


#include "ZObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZObjectPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<UObject*>::Encode(UnderlyingObjectProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZObjectPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	UObject* value = TZCallBufferSlotEncoder<UObject*>::Decode(src);
	check(!value || value->IsA(UnderlyingObjectProperty->PropertyClass));
	UnderlyingProperty->CopySingleValue(dest, &value);
}


