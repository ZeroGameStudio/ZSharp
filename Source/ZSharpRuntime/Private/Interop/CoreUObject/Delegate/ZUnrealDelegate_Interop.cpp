// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealDelegate_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "ZCall/ZCallBuffer.h"

ZSharp::EZCallErrorCode ZSharp::FZUnrealDelegate_Interop::Execute(FZCallBuffer* buffer)
{
	TRY
	{
		FZConjugateHandle self = (*buffer)[0].ReadConjugate();
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		return sdself->Execute(buffer);
	}
	CATCHR(EZCallErrorCode::UnknownError)
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealDelegate_Interop::BindStatelessManagedDelegate(FZConjugateHandle self, FZGCHandle delegate)
{
	TRY
	{
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->BindManaged(delegate));
	}
	CATCHR({})
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealDelegate_Interop::BindStatefulManagedDelegate(FZConjugateHandle self, FZGCHandle delegate, FZGCHandle state)
{
	TRY
	{
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->BindManaged(delegate, state));
	}
	CATCHR({})
}

void ZSharp::FZUnrealDelegate_Interop::BindUnrealFunction(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	GUARD
	(
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		sdself->BindUFunction(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
	);
}

void ZSharp::FZUnrealDelegate_Interop::Unbind(FZConjugateHandle self)
{
	GUARD
	(
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		return sdself->Unbind();
	);
}

uint8 ZSharp::FZUnrealDelegate_Interop::IsBound(FZConjugateHandle self)
{
	TRY
	{
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		return sdself->IsBound();
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealDelegate_Interop::IsBoundToObject(FZConjugateHandle self, FZConjugateHandle object)
{
	TRY
	{
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		return sdself->IsBoundToObject(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object));
	}
	CATCHR(false)
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealDelegate_Interop::GetObject(FZConjugateHandle self)
{
	TRY
	{
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->GetObject());
	}
	CATCHR({})
}

void ZSharp::FZUnrealDelegate_Interop::GetFunctionName(FZConjugateHandle self, FString& functionName)
{
	GUARD
	(
		FZSelfDescriptiveScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Delegate>().ConjugateUnsafe(self);
		functionName = sdself->GetFunctionName().ToString();
	);
}


