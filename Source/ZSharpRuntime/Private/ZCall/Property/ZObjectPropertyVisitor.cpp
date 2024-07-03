// Copyright Zero Games. All Rights Reserved.


#include "ZObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZObjectPropertyVisitor::GetRef_InContainer(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<UObject*>::Encode(UnderlyingObjectProperty->GetObjectPropertyValue_InContainer(src), dest);
}

void ZSharp::FZObjectPropertyVisitor::SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const
{
	UObject* value = TZCallBufferSlotEncoder<UObject*>::Decode(src);
	UnderlyingProperty->SetValue_InContainer(dest, &value);
}


