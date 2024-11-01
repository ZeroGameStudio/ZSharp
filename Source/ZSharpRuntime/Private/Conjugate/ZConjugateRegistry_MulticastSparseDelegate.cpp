// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_MulticastSparseDelegate.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_MulticastSparseDelegate)

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_MulticastSparseDelegate::Conjugate(const USparseDelegateFunction* signature, const FSparseDelegate* unmanaged)
{
	auto mutableUnmanaged = const_cast<FSparseDelegate*>(unmanaged);
	if (const TUniquePtr<FZSelfDescriptiveMulticastSparseScriptDelegate>* pSdsd = ConjugateMap.Find(mutableUnmanaged))
	{
		check((*pSdsd)->GetDescriptor() == signature);
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(signature);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, MakeUnique<FZSelfDescriptiveMulticastSparseScriptDelegate>(signature, mutableUnmanaged));
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate* ZSharp::FZConjugateRegistry_MulticastSparseDelegate::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const TUniquePtr<FZSelfDescriptiveMulticastSparseScriptDelegate>* pSdsd = ConjugateMap.Find(unmanaged);
	return pSdsd ? pSdsd->Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_MulticastSparseDelegate::BuildConjugate(void* userdata)
{
	// There is no black SparseDelegate conjugate.
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZConjugateRegistry_MulticastSparseDelegate::ReleaseConjugate(void* unmanaged)
{
    if (!ConjugateMap.Contains(unmanaged))
    {
    	return;
    }
    
	Alc.ReleaseConjugate(unmanaged);
    ConjugateMap.Remove(unmanaged);
}

void ZSharp::FZConjugateRegistry_MulticastSparseDelegate::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_MulticastSparseDelegate::GetManagedType(const USparseDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}
