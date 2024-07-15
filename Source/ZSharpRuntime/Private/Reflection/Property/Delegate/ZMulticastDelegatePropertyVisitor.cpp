// Copyright Zero Games. All Rights Reserved.


#include "ZMulticastDelegatePropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZConjugateRegistry_MulticastDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastScriptDelegate.h"

void ZSharp::FZMulticastDelegatePropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveMulticastScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastDelegate>().Conjugate(dest.ReadConjugate());
	if (!sdsd)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastDelegate>().Conjugate(UnderlyingDelegateProperty->SignatureFunction, [src, this](const FZSelfDescriptiveMulticastScriptDelegate& sdsd){ UnderlyingProperty->CopySingleValue(sdsd.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
		UnderlyingProperty->CopySingleValue(sdsd->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZMulticastDelegatePropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastDelegate>().Conjugate(UnderlyingDelegateProperty->SignatureFunction, (FMulticastScriptDelegate*)src));
}

void ZSharp::FZMulticastDelegatePropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveMulticastScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastDelegate>().Conjugate(src.ReadConjugate());
	if (!sdsd)
	{
		UnderlyingProperty->InitializeValue(dest);
	}
	
	check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
	UnderlyingProperty->CopySingleValue(dest, sdsd->GetUnderlyingInstance());
}


