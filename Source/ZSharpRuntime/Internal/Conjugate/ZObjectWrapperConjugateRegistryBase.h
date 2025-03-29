// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Reflection/ZReflectionHelper.h"
#include "Trait/ZConjugateKey.h"

namespace ZSharp
{
	template <typename TImpl, typename TConjugate, bool IsGCObject>
	class TZObjectWrapperConjugateRegistryBase : public TZConjugateRegistryBase<TImpl, TConjugate, TConjugate, IsGCObject>
	{

	public:
		using Super = TZConjugateRegistryBase<TImpl, TConjugate, TConjugate, IsGCObject>;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using ConjugateWrapperType = TConjugate;
		using UnderlyingInstanceType = typename TConjugate::UnderlyingInstanceType;

	public:
		explicit TZObjectWrapperConjugateRegistryBase(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		using Super::Conjugate;
		
		FZConjugateHandle Conjugate(const UClass* descriptor) { return Conjugate(descriptor, [](const ConjugateWrapperType&){}); }
		FZConjugateHandle Conjugate(const UClass* descriptor, TFunctionRef<void(const ConjugateWrapperType&)> initialize)
		{
			checkSlow(IsInGameThread());
			
			auto wrapper = new ConjugateWrapperType { descriptor };
			initialize(*wrapper);
			const FZRuntimeTypeHandle type = GetManagedType(descriptor);
			return Super::BuildConjugate_Red(wrapper, type);
		}
		
		FZConjugateHandle Conjugate(const UClass* descriptor, const UnderlyingInstanceType* unmanaged)
		{
			checkSlow(IsInGameThread());
			
			auto mutableUnmanaged = const_cast<UnderlyingInstanceType*>(unmanaged);
			if (const ConjugateWrapperType* wrapper = Super::FindConjugateWrapper(unmanaged))
			{
				check(wrapper->GetDescriptor() == descriptor);
				return { mutableUnmanaged };
			}

			auto wrapper = new ConjugateWrapperType { descriptor, mutableUnmanaged };
			const FZRuntimeTypeHandle type = GetManagedType(descriptor);
			return Super::BuildConjugate_Red(wrapper, type);
		}

	private:
		FZRuntimeTypeHandle GetManagedType(const UClass* cls) const
		{
			FString rootKey = TZConjugateKey<ConjugateType>::Value;
			FZRuntimeTypeUri inner { FZReflectionHelper::GetFieldConjugateKey(cls) };
			FZRuntimeTypeUri uri { rootKey, inner };
	
			return Super::Alc.GetType(uri);
		}
		
	};
}
