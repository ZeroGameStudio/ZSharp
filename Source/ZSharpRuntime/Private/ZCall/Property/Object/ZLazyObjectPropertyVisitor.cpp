// Copyright Zero Games. All Rights Reserved.


#include "ZLazyObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZLazyObjectPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FLazyObjectPtr* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_LazyObjectPtr>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FLazyObjectPtr;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_LazyObjectPtr>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZLazyObjectPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FLazyObjectPtr>::Encode(UnderlyingObjectProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZLazyObjectPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FLazyObjectPtr* value = TZCallBufferSlotEncoder<FLazyObjectPtr>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	check(value->IsNull() || (*value)->IsA(UnderlyingObjectProperty->PropertyClass));
	UnderlyingProperty->CopySingleValue(dest, &value);
}


