// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealMulticastInlineDelegate_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"
#include "ZCall/ZCallBuffer.h"

ZSharp::EZCallErrorCode ZSharp::FZUnrealMulticastInlineDelegate_Interop::Broadcast(FZCallBuffer* buffer)
{
	TRY
	{
		FZConjugateHandle self = (*buffer)[0].ReadConjugate();
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		return sdself->Broadcast(buffer);
	}
	CATCHR(EZCallErrorCode::UnknownError)
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealMulticastInlineDelegate_Interop::AddStatelessManagedDelegate(FZConjugateHandle self, FZGCHandle delegate)
{
	TRY
	{
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->AddManaged(delegate));
	}
	CATCHR({})
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealMulticastInlineDelegate_Interop::AddStatefulManagedDelegate(FZConjugateHandle self, FZGCHandle delegate, FZGCHandle state)
{
	TRY
	{
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->AddManaged(delegate, state));
	}
	CATCHR({})
}

void ZSharp::FZUnrealMulticastInlineDelegate_Interop::AddUnrealFunction(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	GUARD
	(
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		sdself->AddUFunction(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
	);
}

void ZSharp::FZUnrealMulticastInlineDelegate_Interop::Remove(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	GUARD
	(
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		sdself->RemoveUFunction(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
	);
}

void ZSharp::FZUnrealMulticastInlineDelegate_Interop::RemoveAll(FZConjugateHandle self, FZConjugateHandle object)
{
	GUARD
	(
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		sdself->RemoveAll(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object));
	);
}

void ZSharp::FZUnrealMulticastInlineDelegate_Interop::Clear(FZConjugateHandle self)
{
	GUARD
	(
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		sdself->Clear();
	);
}

uint8 ZSharp::FZUnrealMulticastInlineDelegate_Interop::IsBound(FZConjugateHandle self)
{
	TRY
	{
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		return sdself->IsBound();
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealMulticastInlineDelegate_Interop::IsBoundToObject(FZConjugateHandle self, FZConjugateHandle object)
{
	TRY
	{
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		return sdself->IsBoundToObject(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object));
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealMulticastInlineDelegate_Interop::Contains(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	TRY
	{
		FZSelfDescriptiveMulticastInlineScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate>().ConjugateUnsafe(self);
		return sdself->Contains(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
	}
	CATCHR(false)
}


