// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Trait/ZConjugateKey.h"
#include "Misc/ZUnmanagedHeapGCHelper.h"

namespace ZSharp
{
	namespace ZRegularConjugateRegistryBase_Private
	{
		template <typename TConjugate>
		struct TZConjugateWrapper final
		{
			~TZConjugateWrapper()
			{
				if (bOwning)
				{
					delete UnderlyingInstance;
				}
			}
			void AddReferencedObjects(FReferenceCollector& collector)
			{
				if constexpr (std::is_same_v<TConjugate, FFieldPath>)
				{
					FZUnmanagedHeapGCHelper::CollectFieldPathReference(collector, *UnderlyingInstance);
				}
			}
			TConjugate* GetUnderlyingInstance() { return UnderlyingInstance; }
			TConjugate* UnderlyingInstance;
			bool bOwning;
		};
	}

	template <typename TImpl, typename TConjugate, bool IsGCObject>
	class TZRegularConjugateRegistryBase : public TZConjugateRegistryBase<TImpl, TConjugate, ZRegularConjugateRegistryBase_Private::TZConjugateWrapper<TConjugate>, IsGCObject>
	{

	public:
		using Super = TZConjugateRegistryBase<TImpl, TConjugate, ZRegularConjugateRegistryBase_Private::TZConjugateWrapper<TConjugate>, IsGCObject>;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using ConjugateWrapperType = ZRegularConjugateRegistryBase_Private::TZConjugateWrapper<TConjugate>;

	public:
		explicit TZRegularConjugateRegistryBase(IZMasterAssemblyLoadContext& alc) : Super(alc){}
		
	public:
		using Super::Conjugate;
		
		FZConjugateHandle Conjugate(const TConjugate* unmanaged, bool owning)
		{
			checkSlow(IsInGameThread());
			
			auto mutableUnmanaged = const_cast<TConjugate*>(unmanaged);
			if (Super::FindConjugateWrapper(mutableUnmanaged))
			{
				// This branch means conjugating an unmanaged object that doesn't owned by the caller.
				check(!owning);
				return { mutableUnmanaged };
			}

			auto wrapper = new ConjugateWrapperType { mutableUnmanaged, owning };
			const FZRuntimeTypeHandle type = GetManagedType();
			return Super::BuildConjugate_Red(wrapper, type);
		}

	private:
		FZRuntimeTypeHandle GetManagedType() const
		{
			FZRuntimeTypeUri uri { TZConjugateKey<ConjugateType>::Value };
			return Super::Alc.GetType(uri);
		}

	};
}


