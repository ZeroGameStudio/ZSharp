// Copyright Zero Games. All Rights Reserved.


#include "ZStructPropertyVisitor.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/Conjugate/ZConjugateRegistry_UScriptStruct.h"

void ZSharp::FZStructPropertyVisitor::GetValue_InContainer(const void* src, FZCallBufferSlot& dest) const
{
	void const* ref = UnderlyingStructProperty->ContainerPtrToValuePtr<void>(src);
	const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(dest.ReadConjugate());
	const UScriptStruct* scriptStruct = sdss->GetDescriptor();
	check(scriptStruct == UnderlyingStructProperty->Struct);
	void* unmanaged = sdss ? sdss->GetUnderlyingInstance() : nullptr;
	if (!unmanaged)
	{
		unmanaged = FMemory::Malloc(scriptStruct->GetStructureSize(), scriptStruct->GetMinAlignment());
		dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(scriptStruct, unmanaged, true));
	}
	scriptStruct->CopyScriptStruct(unmanaged, ref);
}

void ZSharp::FZStructPropertyVisitor::GetRef_InContainer(const void* src, FZCallBufferSlot& dest) const
{
	void const* ref = UnderlyingStructProperty->ContainerPtrToValuePtr<void>(src);
	const UScriptStruct* scriptStruct = UnderlyingStructProperty->Struct;
	dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(scriptStruct, ref, false));
}

void ZSharp::FZStructPropertyVisitor::SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const
{
	const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(src.ReadConjugate());
	const UScriptStruct* scriptStruct = sdss->GetDescriptor();
	check(scriptStruct == UnderlyingStructProperty->Struct);
	if (void* unmanaged = sdss ? sdss->GetUnderlyingInstance() : nullptr)
	{
		UnderlyingStructProperty->CopyCompleteValue_InContainer(dest, unmanaged);
	}
}


