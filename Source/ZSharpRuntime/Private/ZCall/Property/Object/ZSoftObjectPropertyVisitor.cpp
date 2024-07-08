// Copyright Zero Games. All Rights Reserved.


#include "ZSoftObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZSoftObjectPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FSoftObjectPtr>::Encode(UnderlyingObjectProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZSoftObjectPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	static const FSoftObjectPtr GDefault{};
	
	const FSoftObjectPtr* value = TZCallBufferSlotEncoder<FSoftObjectPtr>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->CopySingleValue(dest, &GDefault);
		return;
	}
	
	check(value->IsNull() || value->LoadSynchronous()->IsA(UnderlyingObjectProperty->PropertyClass));
	UnderlyingProperty->CopySingleValue(dest, &value);
}


