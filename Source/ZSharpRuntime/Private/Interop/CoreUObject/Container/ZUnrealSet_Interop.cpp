// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealSet_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"

namespace ZSharp::ZUnrealSet_Interop_Private
{
	static FZUnrealSet_Interop::FZIterator* CreateEnumerator(FZConjugateHandle target)
	{
		FZSelfDescriptiveScriptSet* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(target);
		if (!sdtarget->Num())
		{
			return nullptr;
		}
	
		auto* it = new FZUnrealSet_Interop::FZIterator { sdtarget->GetHelper() };
		if (!it)
		{
			delete it;
			it = nullptr;
		}

		return it;
	}
}

ZSharp::FZUnrealSet_Interop::FZIterator::FZIterator(const FScriptSetHelper& target)
	: Target(target)
{
	Iterator = MakeUnique<FScriptSetHelper::FIterator>(Target);
}

uint8 ZSharp::FZUnrealSet_Interop::Add(FZConjugateHandle self, FZCallBufferSlot item)
{
	TRY
	{
		FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
		return sdself->Add(item);
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealSet_Interop::Remove(FZConjugateHandle self, FZCallBufferSlot item)
{
	TRY
	{
		FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
		return sdself->Remove(item);
	}
	CATCHR(false)
}

void ZSharp::FZUnrealSet_Interop::Clear(FZConjugateHandle self)
{
	GUARD
	(
		FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
		sdself->Clear();
	);
}

uint8 ZSharp::FZUnrealSet_Interop::Contains(FZConjugateHandle self, FZCallBufferSlot item)
{
	TRY
	{
		FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
		return sdself->Contains(item);
	}
	CATCHR(false)
}

int32 ZSharp::FZUnrealSet_Interop::Count(FZConjugateHandle self)
{
	TRY
	{
		FZSelfDescriptiveScriptSet* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(self);
		return sdself->Num();
	}
	CATCHR(0)
}

ZSharp::FZUnrealSet_Interop::FZIterator* ZSharp::FZUnrealSet_Interop::CreateEnumerator(FZConjugateHandle target)
{
	TRY
	{
		return ZUnrealSet_Interop_Private::CreateEnumerator(target);
	}
	CATCHR(nullptr)
}

void ZSharp::FZUnrealSet_Interop::ReleaseEnumerator(FZIterator* self)
{
	GUARD(delete self);
}

uint8 ZSharp::FZUnrealSet_Interop::EnumeratorMoveNext(FZIterator* self)
{
	TRY
	{
		++*self->Iterator;
		return !!*self->Iterator;
	}
	CATCHR(false)
}

void ZSharp::FZUnrealSet_Interop::EnumeratorCurrent(FZIterator* self, FZConjugateHandle target, FZCallBufferSlot& item)
{
	GUARD
	(
		FZSelfDescriptiveScriptSet* sdtarget = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Set>().ConjugateUnsafe(target);
		sdtarget->Get(*self->Iterator, item);
	);
}


