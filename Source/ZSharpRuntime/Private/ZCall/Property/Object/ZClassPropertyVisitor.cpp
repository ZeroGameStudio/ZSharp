// Copyright Zero Games. All Rights Reserved.


#include "ZClassPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZClassPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<UClass*>::Encode(static_cast<UClass*>(UnderlyingClassProperty->GetPropertyValue(src)), dest);
}

void ZSharp::FZClassPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	UClass* value = TZCallBufferSlotEncoder<UClass*>::Decode(src);
	check(!value || value->IsChildOf(UnderlyingClassProperty->MetaClass));
	UnderlyingProperty->CopySingleValue(dest, &value);
}


