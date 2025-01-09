// Copyright Zero Games. All Rights Reserved.

#include "ZSetPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"

void ZSharp::FZSetPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveScriptSet* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().Conjugate(dest.ReadConjugate());
	if (!sdss)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().Conjugate(UnderlyingSetProperty->ElementProp, [src, this](const FZSelfDescriptiveScriptSet& sdss){ UnderlyingProperty->CopySingleValue(sdss.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(sdss->GetDescriptor()->GetClass() == UnderlyingSetProperty->ElementProp->GetClass());
		UnderlyingProperty->CopySingleValue(sdss->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZSetPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().Conjugate(UnderlyingSetProperty->ElementProp, (FScriptSet*)src));
}

void ZSharp::FZSetPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveScriptSet* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().Conjugate(src.ReadConjugate());
	if (!sdss)
	{
		UnderlyingProperty->InitializeValue(dest);
	}
	
	check(sdss->GetDescriptor()->GetClass() == UnderlyingSetProperty->ElementProp->GetClass());
	UnderlyingProperty->CopySingleValue(dest, sdss->GetUnderlyingInstance());
}


