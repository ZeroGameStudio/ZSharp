// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Delegate.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Delegate)

ZSharp::FZSelfDescriptiveScriptDelegate* ZSharp::FZConjugateRegistry_Delegate::BuildConjugateWrapper(void* userdata)
{
	const auto signature = static_cast<UDelegateFunction*>(userdata);
	return new FZSelfDescriptiveScriptDelegate { signature };
}

void ZSharp::FZConjugateRegistry_Delegate::ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Delegate::GetManagedType(const UDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}




