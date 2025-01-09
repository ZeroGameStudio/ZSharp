// Copyright Zero Games. All Rights Reserved.

#include "ZAnsiStringPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZAnsiStringPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FAnsiString* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FAnsiString;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZAnsiStringPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FAnsiString>::Encode(UnderlyingStringProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZAnsiStringPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FAnsiString* value = TZCallBufferSlotEncoder<FAnsiString>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	UnderlyingProperty->CopySingleValue(dest, value);
}


