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
	template <typename TImpl, typename TConjugate>
	class TZObjectWrapperConjugateRegistryBase : public TZConjugateRegistryBase<TImpl, TConjugate>
	{

	public:
		using Super = TZConjugateRegistryBase<TImpl, TConjugate>;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using ConjugateWrapperType = TConjugate;
		using UnderlyingInstanceType = typename TConjugate::UnderlyingInstanceType;

	public:
		explicit TZObjectWrapperConjugateRegistryBase(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const UClass* descriptor) { return Conjugate(descriptor, [](const ConjugateWrapperType&){}); }
		FZConjugateHandle Conjugate(const UClass* descriptor, TFunctionRef<void(const ConjugateWrapperType&)> initialize)
		{
			check(IsInGameThread());
			
			auto wrapper = new ConjugateWrapperType { descriptor };
			initialize(*wrapper);
			const FZRuntimeTypeHandle type = GetManagedType(descriptor);
			return Super::BuildConjugate_Red(wrapper, type);
		}
		
		FZConjugateHandle Conjugate(const UClass* descriptor, const UnderlyingInstanceType* unmanaged)
		{
			check(IsInGameThread());
			
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
		
		ConjugateType* Conjugate(FZConjugateHandle handle) const { return Super::Conjugate(handle); }

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
