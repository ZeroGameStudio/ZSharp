// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_MulticastInlineDelegate.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_MulticastInlineDelegate)

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate* ZSharp::FZConjugateRegistry_MulticastInlineDelegate::BuildConjugateWrapper(void* userdata)
{
	const auto signature = static_cast<UDelegateFunction*>(userdata);
	return new FZSelfDescriptiveMulticastInlineScriptDelegate { signature };
}

void ZSharp::FZConjugateRegistry_MulticastInlineDelegate::ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveMulticastInlineScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_MulticastInlineDelegate::GetManagedType(const UDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}


