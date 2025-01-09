// Copyright Zero Games. All Rights Reserved.

#include "ZDelegatePropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"

void ZSharp::FZDelegatePropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().Conjugate(dest.ReadConjugate());
	if (!sdsd)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().Conjugate(CastChecked<UDelegateFunction>(UnderlyingDelegateProperty->SignatureFunction), [src, this](const FZSelfDescriptiveScriptDelegate& sdsd){ UnderlyingProperty->CopySingleValue(sdsd.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
		UnderlyingProperty->CopySingleValue(sdsd->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZDelegatePropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().Conjugate(CastChecked<UDelegateFunction>(UnderlyingDelegateProperty->SignatureFunction), (FScriptDelegate*)src));
}

void ZSharp::FZDelegatePropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().Conjugate(src.ReadConjugate());
	if (!sdsd)
	{
		UnderlyingProperty->InitializeValue(dest);
	}
	
	check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
	UnderlyingProperty->CopySingleValue(dest, sdsd->GetUnderlyingInstance());
}


