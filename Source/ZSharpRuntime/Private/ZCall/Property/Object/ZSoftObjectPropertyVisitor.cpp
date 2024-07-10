// Copyright Zero Games. All Rights Reserved.


#include "ZSoftObjectPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZSoftObjectPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FSoftObjectPtr* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_SoftObjectPtr>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FSoftObjectPtr;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_SoftObjectPtr>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZSoftObjectPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FSoftObjectPtr>::Encode(UnderlyingObjectProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZSoftObjectPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FSoftObjectPtr* value = TZCallBufferSlotEncoder<FSoftObjectPtr>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	check(value->IsNull() || value->LoadSynchronous()->IsA(UnderlyingObjectProperty->PropertyClass));
	UnderlyingProperty->CopySingleValue(dest, &value);
}


