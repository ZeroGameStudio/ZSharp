// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Array.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

namespace ZSharp::ZConjugateRegistry_Array_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_Array> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Array::Conjugate(const FProperty* elementPropertyProto, TFunctionRef<void(const FZSelfDescriptiveScriptArray&)> initialize)
{
	FProperty* elementProperty = CastField<FProperty>(FField::Duplicate(elementPropertyProto, nullptr));
	auto sdsa = new FZSelfDescriptiveScriptArray { elementProperty };
	initialize(*sdsa);
	
	void* unmanaged = sdsa->GetUnderlyingInstance();
	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveScriptArray>(sdsa), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Array::Conjugate(const FProperty* elementProperty, const FScriptArray* unmanaged)
{
	const auto mutableUnmanaged = const_cast<FScriptArray*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Array->GetDescriptor()->GetClass() == elementProperty->GetClass());
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveScriptArray>(elementProperty, mutableUnmanaged), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveScriptArray* ZSharp::FZConjugateRegistry_Array::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Array.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_Array::BuildConjugate(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	auto pSdsa = MakeUnique<FZSelfDescriptiveScriptArray>(FZPropertyFactory::Create(typedUserdata.Desc));
	void* unmanaged = pSdsa->GetUnderlyingInstance();
	ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdsa), true });
	return unmanaged;
}

void ZSharp::FZConjugateRegistry_Array::ReleaseConjugate(void* unmanaged)
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

void ZSharp::FZConjugateRegistry_Array::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Array::GetManagedType(const FProperty* elementProperty) const
{
	FZRuntimeTypeLocatorWrapper locator;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FArrayProperty::StaticClass(), locator))
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


