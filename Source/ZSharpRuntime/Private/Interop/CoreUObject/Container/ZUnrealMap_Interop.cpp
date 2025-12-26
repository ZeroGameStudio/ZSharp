// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealMap_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"

namespace ZSharp::ZUnrealMap_Interop_Private
{
	static FZUnrealMap_Interop::FZIterator* CreateEnumerator(FZConjugateHandle target)
	{
		FZSelfDescriptiveScriptMap* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(target);
		if (!sdtarget->Num())
		{
			return nullptr;
		}
	
		auto* it = new FZUnrealMap_Interop::FZIterator { sdtarget->GetHelper() };
		if (!it)
		{
			delete it;
			it = nullptr;
		}

		return it;
	}
}

ZSharp::FZUnrealMap_Interop::FZIterator::FZIterator(const FScriptMapHelper& target)
	: Target(target)
{
	Iterator = MakeUnique<FScriptMapHelper::FIterator>(Target);
}

uint8 ZSharp::FZUnrealMap_Interop::TryAdd(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot value)
{
	TRY
	{
		FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
		return sdself->TryAdd(key, value);
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealMap_Interop::Remove(FZConjugateHandle self, FZCallBufferSlot key)
{
	TRY
	{
		FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
		return sdself->Remove(key);
	}
	CATCHR(false)
}

void ZSharp::FZUnrealMap_Interop::Clear(FZConjugateHandle self)
{
	GUARD
	(
		FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
		sdself->Clear();
	);
}

uint8 ZSharp::FZUnrealMap_Interop::Contains(FZConjugateHandle self, FZCallBufferSlot key)
{
	TRY
	{
		FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
		return sdself->Contains(key);
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealMap_Interop::Get(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot& value)
{
	TRY
	{
		FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
		return sdself->Find(key, value);
	}
	CATCHR(false)
}

void ZSharp::FZUnrealMap_Interop::Set(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot value)
{
	GUARD
	(
		FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
		return sdself->Set(key, value);
	);
}

int32 ZSharp::FZUnrealMap_Interop::Count(FZConjugateHandle self)
{
	TRY
	{
		FZSelfDescriptiveScriptMap* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(self);
		return sdself->Num();
	}
	CATCHR(0)
}

ZSharp::FZUnrealMap_Interop::FZIterator* ZSharp::FZUnrealMap_Interop::CreateEnumerator(FZConjugateHandle target)
{
	TRY
	{
		return ZUnrealMap_Interop_Private::CreateEnumerator(target);
	}
	CATCHR(nullptr)
}

void ZSharp::FZUnrealMap_Interop::ReleaseEnumerator(FZIterator* self)
{
	GUARD(delete self);
}

uint8 ZSharp::FZUnrealMap_Interop::EnumeratorMoveNext(FZIterator* self)
{
	TRY
	{
		++*self->Iterator;
		return !!*self->Iterator;
	}
	CATCHR(false)
}

void ZSharp::FZUnrealMap_Interop::EnumeratorCurrent(FZIterator* self, FZConjugateHandle target, FZCallBufferSlot& key, FZCallBufferSlot& value)
{
	GUARD
	(
		FZSelfDescriptiveScriptMap* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Map>().ConjugateUnsafe(target);
		sdtarget->Get(*self->Iterator, key, value);
	);
}


