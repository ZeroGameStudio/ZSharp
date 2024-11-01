// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Optional.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

namespace ZSharp::ZConjugateRegistry_Optional_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_Optional> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Optional::Conjugate(const FProperty* elementProperty, TFunctionRef<void(const FZSelfDescriptiveOptional&)> initialize)
{
	auto sdo = new FZSelfDescriptiveOptional { elementProperty, false };
	initialize(*sdo);
	
	void* unmanaged = sdo->GetUnderlyingInstance();
	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveOptional>(sdo), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Optional::Conjugate(const FProperty* elementProperty, const void* unmanaged)
{
	auto mutableUnmanaged = const_cast<void*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Optional->GetDescriptor()->GetClass() == elementProperty->GetClass());
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveOptional>(elementProperty, mutableUnmanaged), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveOptional* ZSharp::FZConjugateRegistry_Optional::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Optional.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_Optional::BuildConjugate(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	auto pSdo = MakeUnique<FZSelfDescriptiveOptional>(FZPropertyFactory::Create(typedUserdata.Desc), true);
	void* unmanaged = pSdo->GetUnderlyingInstance();
	ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdo), true });
	return unmanaged;
}

void ZSharp::FZConjugateRegistry_Optional::ReleaseConjugate(void* unmanaged)
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

void ZSharp::FZConjugateRegistry_Optional::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
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


