// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Trait/ZIsStaticExportedClass.h"
#include "ZCall/IZConjugateRegistry.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;

	namespace ZConjugateRegistryBase_Private
	{
		template <typename T>
		struct TZConjugateRec
		{
			T* TypedUnmanaged;
			bool bOwning;
			bool bBlack;
		};

		struct FZRedFrame
        {
        	TArray<void*> CapturedConjugates;
        };

		template <typename T>
		constexpr uint16 GetConjugateId()
		{
			if constexpr (TZIsStaticExportableClass_V<T>)
			{
				return TZConjugateRegistryId_V<T>;
			}
			else if constexpr (std::is_same_v<T, UObject>)
			{
				return GUObjectConjugateRegistryId;
			}
			else
			{
				return 0;
			}
		}
	}

	/**
	 * Template used as base class for conjugate registries.
	 * 
	 * Usage:
	 * class SomeRegistry : public TZConjugateRegistryBase<SomeRegistry, SomeConjugate[, SomeRecord]>
	 * 
	 * Implementation class is supposed to:
	 * 1. Mark base class as friend
	 * 2. Implement public constructor explicit Registry(IZMasterAssemblyLoadContext& alc);
	 * 3. Implement private function FZRuntimeHandle GetManagedType(const TConjugate* unmanaged) const;
	 * 
	 * In addition, if TRec is not ZConjugateRegistryBase_Private::TZConjugateRec<TConjugate>, then implementation class is also supposed to:
	 * 1. Implement private function ConjugateType* GetUnmanaged(const RecordType* rec) const;
	 * 2. Implement private function RecordType BuildRedConjugateRec(ConjugateType* unmanaged, bool bOwning);
	 * 3. Implement private function ConjugateType* BuildBlackConjugateRec();
	 * 4. Implement private function void InternalReleaseConjugate(void* unmanaged, const RecordType* rec);
	 * 
	 * @tparam TImpl Implement type
	 * @tparam TConjugate Conjugate type
	 * @tparam TRec Conjugate record type
	 */
	template <typename TImpl, typename TConjugate, typename TRec = ZConjugateRegistryBase_Private::TZConjugateRec<TConjugate>>
	class TZConjugateRegistryBase : public IZConjugateRegistry
	{

	public:
		using Super = TZConjugateRegistryBase;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using RecordType = TRec;

	public:
		static constexpr bool bRegularRec = std::is_same_v<TRec, ZConjugateRegistryBase_Private::TZConjugateRec<TConjugate>>;
		static constexpr uint16 RegistryId = ZConjugateRegistryBase_Private::GetConjugateId<TConjugate>();
		
	public:
		explicit TZConjugateRegistryBase(IZMasterAssemblyLoadContext& alc)
			: Alc(alc){}
		
	public:
		TConjugate* Conjugate(FZConjugateHandle handle) const
		{
			void* unmanaged = handle.Handle;
			const TRec* rec = ConjugateMap.Find(unmanaged);
			return rec ? BaseGetUnmanaged(rec) : nullptr;
		}
		
		FZConjugateHandle Conjugate(const TConjugate* unmanaged, bool bOwning)
		{
			auto mutableUnmanaged = const_cast<TConjugate*>(unmanaged);
			if (const TRec* rec = ConjugateMap.Find(mutableUnmanaged))
			{
				// This branch means conjugating an unmanaged object that doesn't owned by the caller.
				check(!bOwning);
				return { BaseGetUnmanaged(rec) };
			}

			const FZRuntimeTypeHandle type = const_cast<const TImpl&>(AsImpl()).GetManagedType(unmanaged);
			if (Alc.BuildConjugate(mutableUnmanaged, type))
			{
				if constexpr (bRegularRec)
				{
					ConjugateMap.Emplace(mutableUnmanaged, { mutableUnmanaged, bOwning, false });
					RedStack.Top().CapturedConjugates.Emplace(mutableUnmanaged);
				}
				else
				{
					ConjugateMap.Emplace(mutableUnmanaged, AsImpl().BuildRedConjugateRec(mutableUnmanaged, bOwning));
				}
					
				return { mutableUnmanaged };
			}

			return {};
		}
	
	public:
		virtual void Release() override
		{
			TArray<void*> conjugates;
			for (const auto& pair : ConjugateMap)
			{
				conjugates.Emplace(pair.Key);
			}

			for (const auto& conjugate : conjugates)
			{
				InternalReleaseConjugate(conjugate);
			}
		}
		
		virtual void* BuildConjugate() override
		{
			if constexpr (bRegularRec)
			{
				auto unmanaged = new TConjugate;
				ConjugateMap.Emplace(unmanaged, { unmanaged, true, true });
				return unmanaged;
			}
			else
			{
				return AsImpl().BuildBlackConjugateRec();
			}
		}
		
		virtual void ReleaseConjugate(void* unmanaged) override
		{
			InternalReleaseConjugate(unmanaged);
		}
		
		virtual void PushRedFrame() override
		{
			RedStack.Push({});
		}
		
		virtual void PopRedFrame() override
		{
			ZConjugateRegistryBase_Private::FZRedFrame frame = RedStack.Pop();
			for (const auto& conjugate : frame.CapturedConjugates)
			{
				InternalReleaseConjugate(conjugate);
			}
		}

	protected:
		TConjugate* BaseGetUnmanaged(const TRec* rec) const
		{
			if constexpr (bRegularRec)
			{
				return rec->TypedUnmanaged;
			}
			else
			{
				return AsImpl().GetUnmanaged(rec);
			}
		}
		
		void InternalReleaseConjugate(void* unmanaged)
		{
			const TRec* rec = ConjugateMap.Find(unmanaged);
			if (!rec)
			{
				return;
			}

			if constexpr (bRegularRec)
			{
				if (rec->bOwning)
				{
					delete rec->TypedUnmanaged;
				}

				if (!rec->bBlack)
				{
					Alc.ReleaseConjugate(unmanaged);
				}
			}
			else
			{
				AsImpl().InternalReleaseConjugate(unmanaged, rec);
			}

			ConjugateMap.Remove(unmanaged);
		}

		TImpl& AsImpl() { return *static_cast<TImpl*>(this); }
		const TImpl& AsImpl() const { return *static_cast<const TImpl*>(this); }

	protected:
		IZMasterAssemblyLoadContext& Alc;
		TMap<void*, TRec> ConjugateMap;
		TArray<ZConjugateRegistryBase_Private::FZRedFrame> RedStack;
		
	};
}


