// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Map.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

namespace ZSharp::ZConjugateRegistry_Map_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_Map> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Map::Conjugate(const FProperty* keyProperty, const FProperty* valueProperty, TFunctionRef<void(const FZSelfDescriptiveScriptMap&)> initialize)
{
	auto desc = new TPair<const FProperty*, const FProperty*> { keyProperty, valueProperty };
	auto sdsm = new FZSelfDescriptiveScriptMap { desc, false };
	initialize(*sdsm);
	
	void* unmanaged = sdsm->GetUnderlyingInstance();
	const FZRuntimeTypeHandle type = GetManagedType(keyProperty, valueProperty);
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveScriptMap>(sdsm), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Map::Conjugate(const FProperty* keyProperty, const FProperty* valueProperty, const FScriptMap* unmanaged)
{
	auto mutableUnmanaged = const_cast<FScriptMap*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Map->GetDescriptor()->Key->GetClass() == keyProperty->GetClass());
		check(rec->Map->GetDescriptor()->Value->GetClass() == valueProperty->GetClass());
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(keyProperty, valueProperty);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		auto desc = new TPair<const FProperty*, const FProperty*> { keyProperty, valueProperty };
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveScriptMap>(desc, mutableUnmanaged), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveScriptMap* ZSharp::FZConjugateRegistry_Map::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Map.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_Map::BuildConjugate(void* userdata)
{
	struct
	{
		FZPropertyDesc KeyDesc;
		FZPropertyDesc ValueDesc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	auto desc = new TPair<const FProperty*, const FProperty*> { FZPropertyFactory::Create(typedUserdata.KeyDesc), FZPropertyFactory::Create(typedUserdata.ValueDesc) };
	auto pSdsm = MakeUnique<FZSelfDescriptiveScriptMap>(desc, true);
	void* unmanaged = pSdsm->GetUnderlyingInstance();
	ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdsm), true });
	return unmanaged;
}

void ZSharp::FZConjugateRegistry_Map::ReleaseConjugate(void* unmanaged)
{
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	if (!rec)
	{
		return;
	}
	
	if (!rec->bBlack)
	{
		Alc.ReleaseConjugate(unmanaged);
	}

	ConjugateMap.Remove(unmanaged);
}

void ZSharp::FZConjugateRegistry_Map::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Map::GetManagedType(const FProperty* keyProperty, const FProperty* valueProperty) const
{
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


