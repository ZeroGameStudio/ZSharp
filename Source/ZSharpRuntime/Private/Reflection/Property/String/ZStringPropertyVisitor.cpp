// Copyright Zero Games. All Rights Reserved.


#include "ZStringPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZStringPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FString* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_String>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FString;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_String>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZStringPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FString>::Encode(UnderlyingStringProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZStringPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FString* value = TZCallBufferSlotEncoder<FString>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	UnderlyingProperty->CopySingleValue(dest, value);
}


