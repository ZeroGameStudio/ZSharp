// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Array.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Array)

ZSharp::FZSelfDescriptiveScriptArray* ZSharp::FZConjugateRegistry_Array::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	return new FZSelfDescriptiveScriptArray { FZPropertyFactory::Create(typedUserdata.Desc), true };
}

void ZSharp::FZConjugateRegistry_Array::ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptArray* wrapper)
{
	check(wrapper->GetDescriptor()->GetClass() == elementProperty->GetClass());
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Array::GetManagedType(const FProperty* elementProperty) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FArrayProperty::StaticClass(), uri))
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


