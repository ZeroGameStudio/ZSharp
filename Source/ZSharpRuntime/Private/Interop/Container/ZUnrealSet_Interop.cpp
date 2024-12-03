// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealSet_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"

ZSharp::FZUnrealSet_Interop::FZIterator::FZIterator(const FScriptSetHelper& target)
	: Target(target)
{
	Iterator = MakeUnique<FScriptSetHelper::FIterator>(Target);
}

uint8 ZSharp::FZUnrealSet_Interop::Add(FZConjugateHandle self, FZCallBufferSlot item)
{
	FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
	return sdself->Add(item);
}

uint8 ZSharp::FZUnrealSet_Interop::Remove(FZConjugateHandle self, FZCallBufferSlot item)
{
	FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
	return sdself->Remove(item);
}

void ZSharp::FZUnrealSet_Interop::Clear(FZConjugateHandle self)
{
	FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
	sdself->Clear();
}

uint8 ZSharp::FZUnrealSet_Interop::Contains(FZConjugateHandle self, FZCallBufferSlot item)
{
	FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
	return sdself->Contains(item);
}

int32 ZSharp::FZUnrealSet_Interop::Count(FZConjugateHandle self)
{
	FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
	return sdself->Num();
}

ZSharp::FZUnrealSet_Interop::FZIterator* ZSharp::FZUnrealSet_Interop::CreateEnumerator(FZConjugateHandle target)
{
	FZSelfDescriptiveScriptSet* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(target);
	return new FZIterator { sdtarget->GetHelper() };
}

void ZSharp::FZUnrealSet_Interop::ReleaseEnumerator(FZIterator* self)
{
	delete self;
}

uint8 ZSharp::FZUnrealSet_Interop::EnumeratorMoveNext(FZIterator* self)
{
	++*self->Iterator;
	return EnumeratorIsValid(self);
}

void ZSharp::FZUnrealSet_Interop::EnumeratorCurrent(FZIterator* self, FZConjugateHandle target, FZCallBufferSlot& item)
{
	FZSelfDescriptiveScriptSet* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(target);
	sdtarget->Get(*self->Iterator, item);
}

uint8 ZSharp::FZUnrealSet_Interop::EnumeratorIsValid(FZIterator* self)
{
	return !!*self->Iterator;
}


