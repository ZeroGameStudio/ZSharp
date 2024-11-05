// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_MulticastSparseDelegate.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_MulticastSparseDelegate)

ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate* ZSharp::FZConjugateRegistry_MulticastSparseDelegate::BuildConjugateWrapper(void* userdata)
{
	// There is no black SparseDelegate conjugate.
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZConjugateRegistry_MulticastSparseDelegate::ValidateConjugateWrapper(const USparseDelegateFunction* descriptor, const FZSelfDescriptiveMulticastSparseScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
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
