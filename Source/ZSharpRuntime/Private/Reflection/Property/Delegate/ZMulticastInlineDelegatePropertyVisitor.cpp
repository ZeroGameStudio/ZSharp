// Copyright Zero Games. All Rights Reserved.

#include "ZMulticastInlineDelegatePropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"

void ZSharp::FZMulticastInlineDelegatePropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	const FZSelfDescriptiveMulticastInlineScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().Conjugate(dest.ReadConjugate());
	if (!sdsd)
	{
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().Conjugate(CastChecked<UDelegateFunction>(UnderlyingDelegateProperty->SignatureFunction), [src, this](const FZSelfDescriptiveMulticastInlineScriptDelegate& sdsd){ UnderlyingProperty->CopySingleValue(sdsd.GetUnderlyingInstance(), src); }));
	}
	else
	{
		check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
		UnderlyingProperty->CopySingleValue(sdsd->GetUnderlyingInstance(), src);
	}
}

void ZSharp::FZMulticastInlineDelegatePropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().Conjugate(CastChecked<UDelegateFunction>(UnderlyingDelegateProperty->SignatureFunction), (FMulticastScriptDelegate*)src));
}

void ZSharp::FZMulticastInlineDelegatePropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveMulticastInlineScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().Conjugate(src.ReadConjugate());
	if (!sdsd)
	{
		UnderlyingProperty->InitializeValue(dest);
	}
	
	check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
	UnderlyingProperty->CopySingleValue(dest, sdsd->GetUnderlyingInstance());
}


