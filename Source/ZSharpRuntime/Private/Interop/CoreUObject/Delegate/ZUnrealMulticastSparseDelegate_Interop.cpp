// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealMulticastSparseDelegate_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"
#include "ZCall/ZCallBuffer.h"

ZSharp::EZCallErrorCode ZSharp::FZUnrealMulticastSparseDelegate_Interop::Broadcast(FZCallBuffer* buffer)
{
	FZConjugateHandle self = (*buffer)[0].ReadConjugate();
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	return sdself->Broadcast(buffer);
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealMulticastSparseDelegate_Interop::AddStatelessManagedDelegate(FZConjugateHandle self, FZGCHandle delegate)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->AddManaged(delegate));
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealMulticastSparseDelegate_Interop::AddStatefulManagedDelegate(FZConjugateHandle self, FZGCHandle delegate, FZGCHandle state)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->AddManaged(delegate, state));
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::AddUnrealFunction(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	sdself->AddUFunction(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::Remove(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	sdself->RemoveUFunction(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::RemoveAll(FZConjugateHandle self, FZConjugateHandle object)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	sdself->RemoveAll(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object));
}

void ZSharp::FZUnrealMulticastSparseDelegate_Interop::Clear(FZConjugateHandle self)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	sdself->Clear();
}

uint8 ZSharp::FZUnrealMulticastSparseDelegate_Interop::IsBound(FZConjugateHandle self)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	return sdself->IsBound();
}

uint8 ZSharp::FZUnrealMulticastSparseDelegate_Interop::IsBoundToObject(FZConjugateHandle self, FZConjugateHandle object)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	return sdself->IsBoundToObject(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object));
}

uint8 ZSharp::FZUnrealMulticastSparseDelegate_Interop::Contains(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName)
{
	FZSelfDescriptiveMulticastSparseScriptDelegate* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_MulticastSparseDelegate>().ConjugateUnsafe(self);
	return sdself->Contains(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(object), functionName);
}


