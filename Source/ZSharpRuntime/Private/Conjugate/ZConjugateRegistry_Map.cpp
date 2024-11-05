// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Map.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Map)

ZSharp::FZSelfDescriptiveScriptMap* ZSharp::FZConjugateRegistry_Map::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc KeyDesc;
		FZPropertyDesc ValueDesc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);

	const FProperty* keyProperty = FZPropertyFactory::Create(typedUserdata.KeyDesc);
	FProperty* valueProperty = FZPropertyFactory::Create(typedUserdata.ValueDesc);

	int32 valueOffset = FScriptMap::GetScriptLayout(keyProperty->GetSize(), keyProperty->GetMinAlignment(), valueProperty->GetSize(), valueProperty->GetMinAlignment()).ValueOffset;
	UEProperty_Private::FProperty_DoNotUse::Unsafe_AlterOffset(*valueProperty, valueOffset);
	
	auto desc = new TPair<const FProperty*, const FProperty*> { keyProperty, valueProperty };
	
	return new FZSelfDescriptiveScriptMap { desc, true };
}

void ZSharp::FZConjugateRegistry_Map::ValidateConjugateWrapper(const TPair<const FProperty*, const FProperty*>* descriptor, const FZSelfDescriptiveScriptMap* wrapper)
{
	const FProperty* keyProperty = descriptor->Key;
	const FProperty* valueProperty = descriptor->Value;

	check(wrapper->GetDescriptor()->Key->GetClass() == keyProperty->GetClass());
	check(wrapper->GetDescriptor()->Value->GetClass() == valueProperty->GetClass());
	
	delete descriptor;
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Map::GetManagedType(const TPair<const FProperty*, const FProperty*>* descriptor) const
{
	const FProperty* keyProperty = descriptor->Key;
	const FProperty* valueProperty = descriptor->Value;
	
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FMapProperty::StaticClass(), uri))
	{
		return {};
	}
	
	FZRuntimeTypeUri& inner = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(keyProperty, inner))
	{
		return {};
	}

	FZRuntimeTypeUri& outer = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(valueProperty, outer))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}


