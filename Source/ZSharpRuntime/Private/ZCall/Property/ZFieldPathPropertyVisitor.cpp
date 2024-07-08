// Copyright Zero Games. All Rights Reserved.


#include "ZFieldPathPropertyVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

void ZSharp::FZFieldPathPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	FFieldPath* unmanaged = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_FieldPath>().Conjugate(dest.ReadConjugate());
	if (!unmanaged)
	{
		unmanaged = new FFieldPath;
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_FieldPath>().Conjugate(unmanaged, true));
	}
	UnderlyingProperty->CopySingleValue(unmanaged, src);
}

void ZSharp::FZFieldPathPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	TZCallBufferSlotEncoder<FFieldPath>::Encode(UnderlyingFieldPathProperty->GetPropertyValue(src), dest);
}

void ZSharp::FZFieldPathPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	static const FFieldPath GDefault{};
	
	const FFieldPath* value = TZCallBufferSlotEncoder<FFieldPath>::DecodePointer(src);
	if (!value)
	{
		UnderlyingProperty->CopySingleValue(dest, &GDefault);
		return;
	}
	
	UnderlyingProperty->CopySingleValue(dest, &value);
}


