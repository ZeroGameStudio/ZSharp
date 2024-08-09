// Copyright Zero Games. All Rights Reserved.


#include "ZMapPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZConjugateRegistry_Map.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"

void ZSharp::FZMapPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveScriptMap* sdsm = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().Conjugate(dest.ReadConjugate());
	if (!sdsm)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().Conjugate(UnderlyingMapProperty->KeyProp, UnderlyingMapProperty->ValueProp, [src, this](const FZSelfDescriptiveScriptMap& sdsm){ UnderlyingProperty->CopySingleValue(sdsm.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(sdsm->GetDescriptor()->Key->GetClass() == UnderlyingMapProperty->KeyProp->GetClass());
		check(sdsm->GetDescriptor()->Value->GetClass() == UnderlyingMapProperty->ValueProp->GetClass());
		UnderlyingProperty->CopySingleValue(sdsm->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZMapPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().Conjugate(UnderlyingMapProperty->KeyProp, UnderlyingMapProperty->ValueProp, (FScriptMap*)src));
}

void ZSharp::FZMapPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveScriptMap* sdsm = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().Conjugate(src.ReadConjugate());
	if (!sdsm)
	{
		UnderlyingProperty->InitializeValue(dest);
	}
	
	check(sdsm->GetDescriptor()->Key->GetClass() == UnderlyingMapProperty->KeyProp->GetClass());
	check(sdsm->GetDescriptor()->Value->GetClass() == UnderlyingMapProperty->ValueProp->GetClass());
	UnderlyingProperty->CopySingleValue(dest, sdsm->GetUnderlyingInstance());
}


