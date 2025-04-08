// Copyright Zero Games. All Rights Reserved.

#include "ZStructPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

void ZSharp::FZStructPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(dest.ReadConjugate());
	const UScriptStruct* scriptStruct = UnderlyingStructProperty->Struct;
	if (!sdss)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(scriptStruct, [src, scriptStruct](FZSelfDescriptiveScriptStruct& sdss){ scriptStruct->CopyScriptStruct(sdss.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(UnderlyingStructProperty->Struct->IsChildOf(sdss->GetDescriptor()));
		UnderlyingProperty->CopySingleValue(sdss->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZStructPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	const UScriptStruct* scriptStruct = UnderlyingStructProperty->Struct;
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(scriptStruct, src));
}

void ZSharp::FZStructPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(src.ReadConjugate());
	if (!sdss)
	{
		UnderlyingProperty->InitializeValue(dest);
		return;
	}
	
	check(sdss->GetDescriptor()->IsChildOf(UnderlyingStructProperty->Struct));
	UnderlyingProperty->CopySingleValue(dest, sdss->GetUnderlyingInstance());
}


