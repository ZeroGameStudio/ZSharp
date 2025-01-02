// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealArray_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"

void ZSharp::FZUnrealArray_Interop::Insert(FZConjugateHandle self, int32 index, FZCallBufferSlot item)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	sdself->Insert(index, item);
}

uint8 ZSharp::FZUnrealArray_Interop::Remove(FZConjugateHandle self, FZCallBufferSlot item)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	return sdself->Remove(item);
}

void ZSharp::FZUnrealArray_Interop::RemoveAt(FZConjugateHandle self, int32 index)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	sdself->RemoveAt(index);
}

void ZSharp::FZUnrealArray_Interop::Clear(FZConjugateHandle self)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	sdself->Clear();
}

int32 ZSharp::FZUnrealArray_Interop::IndexOf(FZConjugateHandle self, FZCallBufferSlot item)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	return sdself->IndexOf(item);
}

void ZSharp::FZUnrealArray_Interop::Get(FZConjugateHandle self, int32 index, FZCallBufferSlot& item)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	sdself->Get(index, item);
}

void ZSharp::FZUnrealArray_Interop::Set(FZConjugateHandle self, int32 index, FZCallBufferSlot item)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	sdself->Set(index, item);
}

int32 ZSharp::FZUnrealArray_Interop::Count(FZConjugateHandle self)
{
	FZSelfDescriptiveScriptArray* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Array>().ConjugateUnsafe(self);
	return sdself->Num();
}


