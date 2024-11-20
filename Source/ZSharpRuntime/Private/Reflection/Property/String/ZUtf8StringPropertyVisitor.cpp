// Copyright Zero Games. All Rights Reserved.


#include "ZUtf8StringPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZUtf8StringPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FUtf8String* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Utf8String>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FUtf8String;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Utf8String>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZUtf8StringPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FUtf8String>::Encode(UnderlyingStringProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZUtf8StringPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FUtf8String* value = TZCallBufferSlotEncoder<FUtf8String>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	UnderlyingProperty->CopySingleValue(dest, value);
}


