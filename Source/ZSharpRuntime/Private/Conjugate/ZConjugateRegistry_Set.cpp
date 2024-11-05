// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Set.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Set)

ZSharp::FZSelfDescriptiveScriptSet* ZSharp::FZConjugateRegistry_Set::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	return new FZSelfDescriptiveScriptSet { FZPropertyFactory::Create(typedUserdata.Desc), true };
}

void ZSharp::FZConjugateRegistry_Set::ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptSet* wrapper)
{
	check(wrapper->GetDescriptor()->GetClass() == elementProperty->GetClass());
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Set::GetManagedType(const FProperty* elementProperty) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FSetProperty::StaticClass(), uri))
	{
		return {};
	}
	
	FZRuntimeTypeUri& inner = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(elementProperty, inner))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}


