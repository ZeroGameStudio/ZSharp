// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_MulticastDelegate.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"

namespace ZSharp::ZConjugateRegistry_MulticastDelegate_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_MulticastDelegate> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_MulticastDelegate::Conjugate(const UFunction* signature, TFunctionRef<void(const FZSelfDescriptiveMulticastScriptDelegate&)> initialize)
{
	const FZRuntimeTypeHandle type = GetManagedType(signature);
	FZSelfDescriptiveMulticastScriptDelegate* sdsd = new FZSelfDescriptiveMulticastScriptDelegate { signature };
	initialize(*sdsd);
	void* unmanaged = sdsd->GetUnderlyingInstance();
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveMulticastScriptDelegate>(sdsd), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_MulticastDelegate::Conjugate(const UFunction* signature, const FMulticastScriptDelegate* unmanaged)
{
	const auto mutableUnmanaged = const_cast<FMulticastScriptDelegate*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Delegate->GetDescriptor() == signature);
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(signature);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveMulticastScriptDelegate>(signature, mutableUnmanaged), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveMulticastScriptDelegate* ZSharp::FZConjugateRegistry_MulticastDelegate::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Delegate.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_MulticastDelegate::BuildConjugate(void* userdata)
{
	const UFunction* signature = static_cast<UFunction*>(userdata);

	auto pSdsd = MakeUnique<FZSelfDescriptiveMulticastScriptDelegate>(signature);
    void* unmanaged = pSdsd->GetUnderlyingInstance();
    ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdsd), true });
    return unmanaged;
}

void ZSharp::FZConjugateRegistry_MulticastDelegate::ReleaseConjugate(void* unmanaged)
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

void ZSharp::FZConjugateRegistry_MulticastDelegate::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_MulticastDelegate::GetManagedType(const UFunction* signature) const
{
	FZRuntimeTypeLocatorWrapper locator;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FMulticastDelegateProperty::StaticClass(), locator))
	{
		return {};
	}
	
	FZRuntimeTypeLocatorWrapper& inner = locator.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, inner))
	{
		return {};
	}
	
	return Alc.GetType(locator);
}
