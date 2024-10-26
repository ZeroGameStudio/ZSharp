// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Set.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

namespace ZSharp::ZConjugateRegistry_Set_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_Set> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Set::Conjugate(const FProperty* elementProperty, TFunctionRef<void(const FZSelfDescriptiveScriptSet&)> initialize)
{
	auto sdss = new FZSelfDescriptiveScriptSet { elementProperty, false };
	initialize(*sdss);
	
	void* unmanaged = sdss->GetUnderlyingInstance();
	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveScriptSet>(sdss), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Set::Conjugate(const FProperty* elementProperty, const FScriptSet* unmanaged)
{
	auto mutableUnmanaged = const_cast<FScriptSet*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Set->GetDescriptor()->GetClass() == elementProperty->GetClass());
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveScriptSet>(elementProperty, mutableUnmanaged), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveScriptSet* ZSharp::FZConjugateRegistry_Set::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Set.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_Set::BuildConjugate(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	auto pSdss = MakeUnique<FZSelfDescriptiveScriptSet>(FZPropertyFactory::Create(typedUserdata.Desc), true);
	void* unmanaged = pSdss->GetUnderlyingInstance();
	ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdss), true });
	return unmanaged;
}

void ZSharp::FZConjugateRegistry_Set::ReleaseConjugate(void* unmanaged)
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

void ZSharp::FZConjugateRegistry_Set::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Set::GetManagedType(const FProperty* elementProperty) const
{
	FZRuntimeTypeLocatorWrapper locator;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FSetProperty::StaticClass(), locator))
	{
		return {};
	}
	
	FZRuntimeTypeLocatorWrapper& inner = locator.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(elementProperty, inner))
	{
		return {};
	}
	
	return Alc.GetType(locator);
}


