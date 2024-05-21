// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Trait/ZConjugateRegistryId.h"
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
	}

	/**
	 * Template used as base class for conjugate registries.
	 * 
	 * Usage:
	 * class SomeRegistry : public TZConjugateRegistryBase<SomeRegistry, SomeConjugate[, SomeRecord]>
	 * 
	 * Implementation class is supposed to:
	 * 1. Mark base class as friend
	 * 2. Implement public constructor explicit Registry(IZMasterAssemblyLoadContext& alc)
	 * 3. Implement private function FZRuntimeHandle GetManagedType(const TConjugate* unmanaged) const
	 * 
	 * In addition, if TRec is not ZConjugateRegistryBase_Private::TZConjugateRec<TConjugate>, then implementation class is also supposed to:
	 * 1. Implement private function TConjugate* GetUnmanaged(const TRec* rec) const
	 * 2. Implement private function TRec BuildRedConjugateRec(TConjugate* unmanaged, bool bOwning)
	 * 3. Implement private function TRec BuildBlackConjugateRec(TConjugate* unmanaged)
	 * 4. Implement private function void InternalReleaseConjugate(void* unmanaged, const TRec* rec)
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
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<TConjugate>;
		
	public:
		explicit TZConjugateRegistryBase(IZMasterAssemblyLoadContext& alc)
			: Alc(alc){}
		
	public:
		TConjugate* Conjugate(FZConjugateHandle handle) const
		{
			void* unmanaged = handle.Handle;
			const TRec* rec = ConjugateMap.Find(unmanaged);
			return rec ? GetUnmanaged(rec) : nullptr;
		}
		
		FZConjugateHandle Conjugate(TConjugate* unmanaged, bool bOwning)
		{
			if (const TRec* rec = ConjugateMap.Find(unmanaged))
			{
				// This branch means conjugating an unmanaged object that doesn't owned by the caller.
				check(!bOwning);
				return { GetUnmanaged(rec) };
			}

			FZRuntimeTypeHandle type = const_cast<const TImpl&>(AsImpl()).GetManagedType(const_cast<const TConjugate*>(unmanaged));
			if (Alc.BuildConjugate(unmanaged, type))
			{
				if constexpr (bRegularRec)
				{
					ConjugateMap.Emplace(unmanaged, { unmanaged, bOwning, false });
				}
				else
				{
					ConjugateMap.Emplace(unmanaged, AsImpl().BuildRedConjugateRec(unmanaged, bOwning));
				}
					
				return { unmanaged };
			}

			return {};
		}
	
	private:
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
			auto unmanaged = new TConjugate;
			
			if constexpr (bRegularRec)
			{
				ConjugateMap.Emplace(unmanaged, { unmanaged, true, true });
			}
			else
			{
				ConjugateMap.Emplace(unmanaged, AsImpl().BuildBlackConjugateRec(unmanaged));
			}
			
			return unmanaged;
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

	private:
		TConjugate* GetUnmanaged(const TRec* rec) const
		{
			if constexpr (bRegularRec)
			{
				return rec->TypedUnmanaged;
			}
			else
			{
				return AsImpl().TImpl::GetUnmanaged(rec);
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


