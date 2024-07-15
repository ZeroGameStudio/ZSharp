// Copyright Zero Games. All Rights Reserved.


#include "ZMulticastSparseDelegatePropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZConjugateRegistry_MulticastSparseDelegate.h"

void ZSharp::FZMulticastSparseDelegatePropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	checkNoEntry();
	
	// const FZSelfDescriptiveMulticastSparseScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().Conjugate(dest.ReadConjugate());
	// if (!sdsd)
	// {
	// 	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().Conjugate(CastChecked<USparseDelegateFunction>(UnderlyingDelegateProperty->SignatureFunction), [src, this](const FZSelfDescriptiveMulticastSparseScriptDelegate& sdsd){ UnderlyingProperty->CopySingleValue(sdsd.GetUnderlyingInstance(), src); }));
	// }
	// else
	// {
	// 	check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
	// 	UnderlyingProperty->CopySingleValue(sdsd->GetUnderlyingInstance(), src);
	// }
}

void ZSharp::FZMulticastSparseDelegatePropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().Conjugate(CastChecked<USparseDelegateFunction>(UnderlyingDelegateProperty->SignatureFunction), (FSparseDelegate*)src));
}

void ZSharp::FZMulticastSparseDelegatePropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	checkNoEntry();
	
	// const FZSelfDescriptiveMulticastSparseScriptDelegate* sdsd = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().Conjugate(src.ReadConjugate());
	// if (!sdsd)
	// {
	// 	UnderlyingProperty->InitializeValue(dest);
	// }
	//
	// check(sdsd->GetDescriptor() == UnderlyingDelegateProperty->SignatureFunction);
	// UnderlyingProperty->CopySingleValue(dest, sdsd->GetUnderlyingInstance());
}


