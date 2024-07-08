// Copyright Zero Games. All Rights Reserved.


#include "ZWeakObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZWeakObjectPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FWeakObjectPtr>::Encode(UnderlyingObjectProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZWeakObjectPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	static const FWeakObjectPtr GDefault{};
	
	const FWeakObjectPtr* value = TZCallBufferSlotEncoder<FWeakObjectPtr>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->CopySingleValue(dest, &GDefault);
		return;
	}
	
	check(value->IsExplicitlyNull() || value->Get()->IsA(UnderlyingObjectProperty->PropertyClass));
	UnderlyingProperty->CopySingleValue(dest, &value);
}


