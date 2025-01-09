// Copyright Zero Games. All Rights Reserved.

#include "ZNamePropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZNamePropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FName* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Name>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FName;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Name>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZNamePropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FName>::Encode(UnderlyingNameProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZNamePropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FName* value = TZCallBufferSlotEncoder<FName>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	UnderlyingProperty->CopySingleValue(dest, value);
}


