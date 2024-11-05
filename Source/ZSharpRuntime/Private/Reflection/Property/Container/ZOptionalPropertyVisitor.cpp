// Copyright Zero Games. All Rights Reserved.


#include "ZOptionalPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"

void ZSharp::FZOptionalPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveOptional* sdo = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().Conjugate(dest.ReadConjugate());
	if (!sdo)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().Conjugate(UnderlyingOptionalProperty->GetValueProperty(), [src, this](const FZSelfDescriptiveOptional& sdo){ UnderlyingProperty->CopySingleValue(sdo.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(sdo->GetDescriptor()->GetClass() == UnderlyingOptionalProperty->GetValueProperty()->GetClass());
		UnderlyingProperty->CopySingleValue(sdo->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZOptionalPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().Conjugate(UnderlyingOptionalProperty->GetValueProperty(), src));
}

void ZSharp::FZOptionalPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveOptional* sdo = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().Conjugate(src.ReadConjugate());
	if (!sdo)
	{
		UnderlyingProperty->InitializeValue(dest);
	}
	
	check(sdo->GetDescriptor()->GetClass() == UnderlyingOptionalProperty->GetValueProperty()->GetClass());
	UnderlyingProperty->CopySingleValue(dest, sdo->GetUnderlyingInstance());
}


