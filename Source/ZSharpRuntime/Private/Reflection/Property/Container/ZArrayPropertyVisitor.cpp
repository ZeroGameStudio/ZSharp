﻿// Copyright Zero Games. All Rights Reserved.

#include "ZArrayPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"

void ZSharp::FZArrayPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveScriptArray* sdsa = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(dest.ReadConjugate());
	if (!sdsa)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(UnderlyingArrayProperty->Inner, [src, this](FZSelfDescriptiveScriptArray& sdsa){ UnderlyingProperty->CopySingleValue(sdsa.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(sdsa->GetDescriptor()->GetClass() == UnderlyingArrayProperty->Inner->GetClass());
		UnderlyingProperty->CopySingleValue(sdsa->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZArrayPropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(UnderlyingArrayProperty->Inner, (FScriptArray*)src));
}

void ZSharp::FZArrayPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveScriptArray* sdsa = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().Conjugate(src.ReadConjugate());
	if (!sdsa)
	{
		UnderlyingProperty->InitializeValue(dest);
	}
	
	check(sdsa->GetDescriptor()->GetClass() == UnderlyingArrayProperty->Inner->GetClass());
	UnderlyingProperty->CopySingleValue(dest, sdsa->GetUnderlyingInstance());
}


