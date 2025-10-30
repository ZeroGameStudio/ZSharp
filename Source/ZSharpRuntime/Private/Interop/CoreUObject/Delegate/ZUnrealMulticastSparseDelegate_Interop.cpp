// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealMulticastSparseDelegate_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"
#include "ZCall/ZCallBuffer.h"

ZSharp::EZCallErrorCode ZSharp::FZUnrealMulticastSparseDelegate_Interop::Broadcast(FZCallBuffer* buffer)
{
	TRY
	{
		FZConjugateHandle self = (*buffer)[0].ReadConjugate();
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		return sdself->Broadcast(buffer);
	}
	CATCHR(EZCallErrorCode::UnknownError)
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealMulticastSparseDelegate_Interop::AddStatelessManagedDelegate(FZConjugateHandle self, FZGCHandle delegate)
{
	TRY
	{
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->AddManaged(delegate));
	}
	CATCHR({})
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealMulticastSparseDelegate_Interop::AddStatefulManagedDelegate(FZConjugateHandle self, FZGCHandle delegate, FZGCHandle state)
{
	TRY
	{
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->AddManaged(delegate, state));
	}
	CATCHR({})
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::AddUnrealFunction(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	GUARD
	(
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		sdself->AddUFunction(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
	);
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::Remove(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	GUARD
	(
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		sdself->RemoveUFunction(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
	);
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::RemoveAll(FZConjugateHandle self, FZConjugateHandle object)
{
	GUARD
	(
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		sdself->RemoveAll(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object));
	);
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::Clear(FZConjugateHandle self)
{
	GUARD
	(
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		sdself->Clear();
	);
}

uint8 ZSharp::FZUnrealMulticastSparseDelegate_Interop::IsBound(FZConjugateHandle self)
{
	TRY
	{
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		return sdself->IsBound();
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealMulticastSparseDelegate_Interop::IsBoundToObject(FZConjugateHandle self, FZConjugateHandle object)
{
	TRY
	{
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		return sdself->IsBoundToObject(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object));
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealMulticastSparseDelegate_Interop::Contains(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	TRY
	{
		FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
		return sdself->Contains(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
	}
	CATCHR(false)
}


