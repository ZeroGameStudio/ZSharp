// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"

namespace ZSharp
{
	template <typename TImpl, typename TConjugate, bool IsGCObject = false>
	class TZStrangeConjugateRegistryBase : public TZConjugateRegistryBase<TImpl, TConjugate, TConjugate, IsGCObject>
	{
	
	public:
		using Super = TZConjugateRegistryBase<TImpl, TConjugate, TConjugate, IsGCObject>;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using ConjugateWrapperType = TConjugate;
		using DescriptorType = typename ConjugateWrapperType::DescriptorType;
		using UnderlyingInstanceType = typename ConjugateWrapperType::UnderlyingInstanceType;

	public:
		TZStrangeConjugateRegistryBase(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		using Super::Conjugate;
		
		FZConjugateHandle Conjugate(const DescriptorType* descriptor, TFunctionRef<void(ConjugateType&)> initialize = [](ConjugateType&){})
		{
			checkSlow(IsInGameThread());
			
			ConjugateType* wrapper;
			if constexpr (TConjugate::IsUObjectDescriptor)
			{
				wrapper = new ConjugateType { descriptor };
			}
			else
			{
				wrapper = new ConjugateType { descriptor, false };
			}

			initialize(*wrapper);
			const FZRuntimeTypeHandle type = Super::AsImpl().GetManagedType(descriptor);
			return Super::BuildConjugate_Red(wrapper, type);
		}
		
        FZConjugateHandle Conjugate(const DescriptorType* descriptor, const UnderlyingInstanceType* unmanaged)
		{
			checkSlow(IsInGameThread());
			
			auto mutableUnmanaged = const_cast<UnderlyingInstanceType*>(unmanaged);
			if (const ConjugateWrapperType* wrapper = Super::FindConjugateWrapper(mutableUnmanaged))
			{
				ThisClass::ValidateConjugateWrapper(descriptor, wrapper);
				return { mutableUnmanaged };
			}

			auto wrapper = new ConjugateType { descriptor, mutableUnmanaged };
			const FZRuntimeTypeHandle type = Super::AsImpl().GetManagedType(descriptor);
			return Super::BuildConjugate_Red(wrapper, type);
		}

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<ConjugateType>;

	};
}


