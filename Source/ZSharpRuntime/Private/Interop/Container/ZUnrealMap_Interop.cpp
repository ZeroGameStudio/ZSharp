// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealMap_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"

ZSharp::FZUnrealMap_Interop::FZIterator::FZIterator(const FScriptMapHelper& target)
	: Target(target)
{
	Iterator = MakeUnique<FScriptMapHelper::FIterator>(Target);
}

uint8 ZSharp::FZUnrealMap_Interop::TryAdd(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot value)
{
	FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
	return sdself->TryAdd(key, value);
}

uint8 ZSharp::FZUnrealMap_Interop::Remove(FZConjugateHandle self, FZCallBufferSlot key)
{
	FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
	return sdself->Remove(key);
}

void ZSharp::FZUnrealMap_Interop::Clear(FZConjugateHandle self)
{
	FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
	sdself->Clear();
}

uint8 ZSharp::FZUnrealMap_Interop::Contains(FZConjugateHandle self, FZCallBufferSlot key)
{
	FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
	return sdself->Contains(key);
}

bool ZSharp::FZUnrealMap_Interop::Get(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot& value)
{
	FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
	return sdself->Find(key, value);
}

void ZSharp::FZUnrealMap_Interop::Set(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot value)
{
	FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
	return sdself->Set(key, value);
}

int32 ZSharp::FZUnrealMap_Interop::Count(FZConjugateHandle self)
{
	FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
	return sdself->Num();
}

ZSharp::FZUnrealMap_Interop::FZIterator* ZSharp::FZUnrealMap_Interop::CreateEnumerator(FZConjugateHandle target)
{
	FZSelfDescriptiveScriptMap* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(target);
	if (!sdtarget->Num())
	{
		return nullptr;
	}
	
	auto* it = new FZIterator { sdtarget->GetHelper() };
	if (!it)
	{
		delete it;
		it = nullptr;
	}

	return it;
}

void ZSharp::FZUnrealMap_Interop::ReleaseEnumerator(FZIterator* self)
{
	delete self;
}

uint8 ZSharp::FZUnrealMap_Interop::EnumeratorMoveNext(FZIterator* self)
{
	++*self->Iterator;
	return !!*self->Iterator;
}

void ZSharp::FZUnrealMap_Interop::EnumeratorCurrent(FZIterator* self, FZConjugateHandle target, FZCallBufferSlot& key, FZCallBufferSlot& value)
{
	FZSelfDescriptiveScriptMap* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(target);
	sdtarget->Get(*self->Iterator, key, value);
}


