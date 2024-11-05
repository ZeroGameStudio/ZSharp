// Copyright Zero Games. All Rights Reserved.


#include "ZMulticastSparseDelegatePropertyVisitor.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

void ZSharp::FZMulticastSparseDelegatePropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	UE_LOG(LogZSharpRuntime, Fatal, TEXT("Getting copy of sparse delegate is NEVER allowed!!!"));
}

void ZSharp::FZMulticastSparseDelegatePropertyVisitor::GetRef(const void* src, FZCallBufferSlot& dest) const
{
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().Conjugate(CastChecked<USparseDelegateFunction>(UnderlyingDelegateProperty->SignatureFunction), (FSparseDelegate*)src));
}

void ZSharp::FZMulticastSparseDelegatePropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	UE_LOG(LogZSharpRuntime, Fatal, TEXT("Setting value of sparse delegate is NEVER allowed!!!"));
}


