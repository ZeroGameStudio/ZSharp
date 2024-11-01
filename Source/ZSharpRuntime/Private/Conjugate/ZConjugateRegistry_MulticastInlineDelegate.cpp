// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_MulticastInlineDelegate.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"

namespace ZSharp::ZConjugateRegistry_MulticastInlineDelegate_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_MulticastInlineDelegate> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_MulticastInlineDelegate::Conjugate(const UDelegateFunction* signature, TFunctionRef<void(const FZSelfDescriptiveMulticastInlineScriptDelegate&)> initialize)
{
	auto sdsd = new FZSelfDescriptiveMulticastInlineScriptDelegate { signature };
	initialize(*sdsd);
	
	void* unmanaged = sdsd->GetUnderlyingInstance();
	const FZRuntimeTypeHandle type = GetManagedType(signature);
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveMulticastInlineScriptDelegate>(sdsd), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_MulticastInlineDelegate::Conjugate(const UDelegateFunction* signature, const FMulticastScriptDelegate* unmanaged)
{
	auto mutableUnmanaged = const_cast<FMulticastScriptDelegate*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Delegate->GetDescriptor() == signature);
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(signature);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveMulticastInlineScriptDelegate>(signature, mutableUnmanaged), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate* ZSharp::FZConjugateRegistry_MulticastInlineDelegate::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Delegate.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_MulticastInlineDelegate::BuildConjugate(void* userdata)
{
	const auto signature = static_cast<UDelegateFunction*>(userdata);

	auto pSdsd = MakeUnique<FZSelfDescriptiveMulticastInlineScriptDelegate>(signature);
    void* unmanaged = pSdsd->GetUnderlyingInstance();
    ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdsd), true });
    return unmanaged;
}

void ZSharp::FZConjugateRegistry_MulticastInlineDelegate::ReleaseConjugate(void* unmanaged)
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

void ZSharp::FZConjugateRegistry_MulticastInlineDelegate::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_MulticastInlineDelegate::GetManagedType(const UFunction* signature) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}
