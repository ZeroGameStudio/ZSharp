// Copyright Zero Games. All Rights Reserved.


#include "ZWeakObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZWeakObjectPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FWeakObjectPtr* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FWeakObjectPtr;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

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


