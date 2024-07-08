// Copyright Zero Games. All Rights Reserved.


#include "ZLazyObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZLazyObjectPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FLazyObjectPtr>::Encode(UnderlyingObjectProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZLazyObjectPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FLazyObjectPtr& value = TZCallBufferSlotEncoder<FLazyObjectPtr>::Decode(src);
	check(value.IsNull() || value->IsA(UnderlyingObjectProperty->PropertyClass));
	UnderlyingProperty->CopySingleValue(dest, &value);
}


