// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Array.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Array)

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Array::Conjugate(const FProperty* elementProperty, TFunctionRef<void(const FZSelfDescriptiveScriptArray&)> initialize)
{
	auto sdsa = new FZSelfDescriptiveScriptArray { elementProperty, false };
	initialize(*sdsa);
	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	return BuildConjugate_Red(sdsa, type);
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Array::Conjugate(const FProperty* elementProperty, FScriptArray* unmanaged)
{
	if (const FZSelfDescriptiveScriptArray* sdsa = FindConjugateWrapper(unmanaged))
	{
		check(sdsa->GetDescriptor()->GetClass() == elementProperty->GetClass());
		return { unmanaged };
	}

	auto sdsa = new FZSelfDescriptiveScriptArray { elementProperty, unmanaged };
	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	return BuildConjugate_Red(sdsa, type);
}

ZSharp::FZSelfDescriptiveScriptArray* ZSharp::FZConjugateRegistry_Array::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	return new FZSelfDescriptiveScriptArray { FZPropertyFactory::Create(typedUserdata.Desc), true };
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


