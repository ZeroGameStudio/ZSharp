// Copyright Zero Games. All Rights Reserved.


#include "ZTextPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZTextPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FText* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Text>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FText;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Text>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZTextPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FText>::Encode(UnderlyingTextProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZTextPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FText* value = TZCallBufferSlotEncoder<FText>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	UnderlyingProperty->CopySingleValue(dest, &value);
}


