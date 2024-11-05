// Copyright Zero Games. All Rights Reserved.


#include "ZMapPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"

void ZSharp::FZMapPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveScriptMap* sdsm = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().Conjugate(dest.ReadConjugate());
	if (!sdsm)
	{
		const auto desc = new TPair<const FProperty*, const FProperty*> { UnderlyingMapProperty->KeyProp, UnderlyingMapProperty->ValueProp };
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().Conjugate(desc, [src, this](const FZSelfDescriptiveScriptMap& sdsm){ UnderlyingProperty->CopySingleValue(sdsm.GetUnderlyingInstance(), src); }));
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
	const auto desc = new TPair<const FProperty*, const FProperty*> { UnderlyingMapProperty->KeyProp, UnderlyingMapProperty->ValueProp };
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().Conjugate(desc, (FScriptMap*)src));
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


