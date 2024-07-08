// Copyright Zero Games. All Rights Reserved.


#include "ZSoftObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZSoftObjectPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FSoftObjectPtr>::Encode(UnderlyingObjectProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZSoftObjectPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FSoftObjectPtr& value = TZCallBufferSlotEncoder<FSoftObjectPtr>::Decode(src);
	check(value.IsNull() || value.LoadSynchronous()->IsA(UnderlyingObjectProperty->PropertyClass));
	UnderlyingProperty->CopyCompleteValue(dest, &value);
}


