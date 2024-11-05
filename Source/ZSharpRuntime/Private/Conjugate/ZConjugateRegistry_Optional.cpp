// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Optional.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Optional)

ZSharp::FZSelfDescriptiveOptional* ZSharp::FZConjugateRegistry_Optional::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	return new FZSelfDescriptiveOptional { FZPropertyFactory::Create(typedUserdata.Desc), true };
}

void ZSharp::FZConjugateRegistry_Optional::ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveOptional* wrapper)
{
	check(wrapper->GetDescriptor()->GetClass() == elementProperty->GetClass());
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Optional::GetManagedType(const FProperty* elementProperty) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FOptionalProperty::StaticClass(), uri))
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


