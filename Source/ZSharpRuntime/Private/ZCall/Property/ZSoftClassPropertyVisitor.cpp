// Copyright Zero Games. All Rights Reserved.


#include "ZSoftClassPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZSoftClassPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	// We assume that FSoftClassPtr has the same memory layout as FSoftObjectPtr, so does the engine...
	TZCallBufferSlotEncoder<FSoftClassPtr>::Encode(reinterpret_cast<FSoftClassPtr const&>(UnderlyingClassProperty->GetPropertyValue(src)), dest);
}

void ZSharp::FZSoftClassPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FSoftClassPtr& value = TZCallBufferSlotEncoder<FSoftClassPtr>::Decode(src);
	check(value.IsNull() || value.LoadSynchronous()->IsChildOf(UnderlyingClassProperty->MetaClass));
	UnderlyingProperty->CopyCompleteValue(dest, &value);
}


