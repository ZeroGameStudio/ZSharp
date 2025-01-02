// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"

namespace ZSharp
{
	template <typename TImpl, typename TConjugate>
	class TZStrangeConjugateRegistryBase : public TZConjugateRegistryBase<TImpl, TConjugate>
	{
	
	public:
		using Super = TZConjugateRegistryBase<TImpl, TConjugate>;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using ConjugateWrapperType = TConjugate;
		using DescriptorType = typename ConjugateWrapperType::DescriptorType;
		using UnderlyingInstanceType = typename ConjugateWrapperType::UnderlyingInstanceType;

	public:
		TZStrangeConjugateRegistryBase(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const DescriptorType* descriptor, TFunctionRef<void(const ConjugateType&)> initialize = [](const ConjugateType&){})
		{
			check(IsInGameThread());
			
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
			const FZRuntimeTypeHandle type = AsImpl().GetManagedType(descriptor);
			return Super::BuildConjugate_Red(wrapper, type);
		}
		
        FZConjugateHandle Conjugate(const DescriptorType* descriptor, const UnderlyingInstanceType* unmanaged)
		{
			check(IsInGameThread());
			
			const auto mutableUnmanaged = const_cast<UnderlyingInstanceType*>(unmanaged);
			if (const ConjugateWrapperType* wrapper = Super::FindConjugateWrapper(mutableUnmanaged))
			{
				ThisClass::ValidateConjugateWrapper(descriptor, wrapper);
				return { mutableUnmanaged };
			}

			const auto wrapper = new ConjugateType { descriptor, mutableUnmanaged };
			const FZRuntimeTypeHandle type = AsImpl().GetManagedType(descriptor);
			return Super::BuildConjugate_Red(wrapper, type);
		}

		ConjugateType* Conjugate(FZConjugateHandle handle) const { return Super::Conjugate(handle); }

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<ConjugateType>;

	private:
		TImpl& AsImpl() { return static_cast<TImpl&>(*this); }

	};
}


