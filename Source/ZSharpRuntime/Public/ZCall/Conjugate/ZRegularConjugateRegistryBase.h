// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Trait/ZIsStaticExportedClass.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;

	namespace ZRegularConjugateRegistryBase_Private
	{
		template <typename T>
		struct TZConjugateRec
		{
			T* TypedUnmanaged;
			bool bOwning;
			bool bBlack;
		};

		template <typename T>
		requires TZIsStaticExportableClass_V<T>
		constexpr uint16 GetConjugateId()
		{
			return TZConjugateRegistryId_V<T>;
		}
	}

	/**
	 * Template used as base class for regular conjugate registries.
	 * 
	 * Usage:
	 * class SomeRegistry : public TZRegularConjugateRegistryBase<SomeRegistry, SomeConjugate[, SomeRecord]>
	 * 
	 * Implementation class is supposed to:
	 * 1. Mark base class as friend
	 * 2. Implement public constructor explicit Registry(IZMasterAssemblyLoadContext& alc);
	 * 3. Implement private function FZRuntimeHandle GetManagedType(const TConjugate* unmanaged) const;
	 * 
	 * @tparam TImpl Implement type
	 * @tparam TConjugate Conjugate type
	 */
	template <typename TImpl, typename TConjugate>
	class TZRegularConjugateRegistryBase : public FZConjugateRegistryBase
	{

	public:
		using Super = TZRegularConjugateRegistryBase;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using RecordType = ZRegularConjugateRegistryBase_Private::TZConjugateRec<TConjugate>;

	public:
		static constexpr uint16 RegistryId = ZRegularConjugateRegistryBase_Private::GetConjugateId<TConjugate>();
		
	public:
		explicit TZRegularConjugateRegistryBase(IZMasterAssemblyLoadContext& alc)
			: FZConjugateRegistryBase(alc){}
		
	public:
		TConjugate* Conjugate(FZConjugateHandle handle) const
		{
			void* unmanaged = handle.Handle;
			const RecordType* rec = ConjugateMap.Find(unmanaged);
			return rec ? rec->TypedUnmanaged : nullptr;
		}
		
		FZConjugateHandle Conjugate(const TConjugate* unmanaged, bool owning)
		{
			const auto mutableUnmanaged = const_cast<TConjugate*>(unmanaged);
			if (const RecordType* rec = ConjugateMap.Find(mutableUnmanaged))
			{
				// This branch means conjugating an unmanaged object that doesn't owned by the caller.
				check(!owning);
				return { rec->TypedUnmanaged };
			}

			const FZRuntimeTypeHandle type = const_cast<const TImpl&>(AsImpl()).GetManagedType(unmanaged);
			if (Alc.BuildConjugate(mutableUnmanaged, type))
			{
				ConjugateMap.Emplace(mutableUnmanaged, { mutableUnmanaged, owning, false });
				CaptureConjugate(mutableUnmanaged);
					
				return { mutableUnmanaged };
			}

			return {};
		}
	
	protected:
		virtual void* BuildConjugate(void* userdata) override
		{
			auto unmanaged = new TConjugate;
			ConjugateMap.Emplace(unmanaged, { unmanaged, true, true });
			return unmanaged;
		}
		
		virtual void ReleaseConjugate(void* unmanaged) override
		{
			const RecordType* rec = ConjugateMap.Find(unmanaged);
			if (!rec)
			{
				return;
			}

			if (rec->bOwning)
			{
				delete rec->TypedUnmanaged;
			}

			if (!rec->bBlack)
			{
				Alc.ReleaseConjugate(unmanaged);
			}

			ConjugateMap.Remove(unmanaged);
		}
		
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override
		{
			for (const auto& pair : ConjugateMap)
			{
				outConjugates.Emplace(pair.Key);
			}
		}

	protected:
		TImpl& AsImpl() { return *static_cast<TImpl*>(this); }

	protected:
		TMap<void*, RecordType> ConjugateMap;
		
	};
}


