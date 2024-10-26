// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_Delegate.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"

namespace ZSharp::ZConjugateRegistry_Delegate_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_Delegate> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Delegate::Conjugate(const UDelegateFunction* signature, TFunctionRef<void(const FZSelfDescriptiveScriptDelegate&)> initialize)
{
	auto sdsd = new FZSelfDescriptiveScriptDelegate { signature };
	initialize(*sdsd);
	
	void* unmanaged = sdsd->GetUnderlyingInstance();
	const FZRuntimeTypeHandle type = GetManagedType(signature);
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveScriptDelegate>(sdsd), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Delegate::Conjugate(const UDelegateFunction* signature, const FScriptDelegate* unmanaged)
{
	auto mutableUnmanaged = const_cast<FScriptDelegate*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Delegate->GetDescriptor() == signature);
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(signature);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveScriptDelegate>(signature, mutableUnmanaged), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveScriptDelegate* ZSharp::FZConjugateRegistry_Delegate::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Delegate.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_Delegate::BuildConjugate(void* userdata)
{
	const auto signature = static_cast<UDelegateFunction*>(userdata);

	auto pSdsd = MakeUnique<FZSelfDescriptiveScriptDelegate>(signature);
    void* unmanaged = pSdsd->GetUnderlyingInstance();
    ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdsd), true });
    return unmanaged;
}

void ZSharp::FZConjugateRegistry_Delegate::ReleaseConjugate(void* unmanaged)
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

void ZSharp::FZConjugateRegistry_Delegate::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Delegate::GetManagedType(const UFunction* signature) const
{
	FZRuntimeTypeLocatorWrapper locator;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, locator))
	{
		return {};
	}
	
	return Alc.GetType(locator);
}
