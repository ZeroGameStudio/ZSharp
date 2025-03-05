// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateMacros.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Conjugate/IZConjugateRegistry.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Trait/ZConjugateRegistryId.h"

namespace ZSharp
{
	namespace ZConjugateRegistryBase_Private
	{
		class IZGCObject
		{
		public:
			virtual void AddReferencedObjects(FReferenceCollector& collector){}
			virtual FString GetReferencerName() const { return {}; }
		public:
			virtual ~IZGCObject() = default;
		};

		// We need this because conjugate registry starts before UObject system.
		class FZLazyReferencer
		{

		public:
			explicit FZLazyReferencer(IZGCObject* outer) : Outer(outer){}
				
		public:
			void Activate()
			{
				if (!Inner)
				{
					Inner = MakeUnique<FZInnerReferencer>();
					Inner->Owner = this;
				}
			}
			
		private:
			struct FZInnerReferencer : public FGCObject
			{
				virtual void AddReferencedObjects(FReferenceCollector& collector) override { Owner->Outer->AddReferencedObjects(collector); }
				virtual FString GetReferencerName() const override { return Owner->Outer->GetReferencerName(); }
				FZLazyReferencer* Owner;
			};

		private:
			IZGCObject* Outer;
			TUniquePtr<FZInnerReferencer> Inner;
			
		};
	}

	template <typename TImpl, typename TConjugate, typename TConjugateWrapper = TConjugate, bool IsGCObject = false>
	class TZConjugateRegistryBase : public IZConjugateRegistry, public FNoncopyable, public ZConjugateRegistryBase_Private::IZGCObject
	{

	public:
		using Super = TZConjugateRegistryBase;
		using ThisClass = TImpl;
		using ConjugateType = TConjugate;
		using ConjugateWrapperType = TConjugateWrapper;
		using UnderlyingInstanceType = std::invoke_result_t<decltype(&ConjugateWrapperType::GetUnderlyingInstance), ConjugateWrapperType>;

	public:
		explicit TZConjugateRegistryBase(IZMasterAssemblyLoadContext& alc)
			: Alc(alc)
			, Referencer(this){}

	public:
		ConjugateType* ConjugateUnsafe(FZConjugateHandle handle) const
		{
			if constexpr (std::is_same_v<UnderlyingInstanceType, ConjugateType> && ZSHARP_ENABLE_CONJUGATE_UNSAFE)
			{
				return static_cast<ConjugateType*>(handle.Handle);
			}
			else
			{
				return Conjugate(handle);
			}
		}
		
		ConjugateType* Conjugate(FZConjugateHandle handle) const
		{
			if (!handle)
			{
				return nullptr;
			}
			
			const void* unmanaged = handle.Handle;
			const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
			if (!UNLIKELY(rec))
			{
				return nullptr;
			}

			if constexpr (std::is_same_v<ConjugateType, ConjugateWrapperType>)
			{
				return rec->Conjugate.Get();
			}
			else
			{
				return rec->Conjugate->GetUnderlyingInstance();
			}
		}
		
	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<ConjugateType>;

	protected:
		FZConjugateHandle BuildConjugate_Red(ConjugateWrapperType* rawWrapper, FZRuntimeTypeHandle managedType)
		{
			auto wrapper = TUniquePtr<ConjugateWrapperType> { rawWrapper };
			void* unmanaged = wrapper->GetUnderlyingInstance();
			if (LIKELY(Alc.BuildConjugate(unmanaged, managedType)))
			{
				ConjugateMap.Emplace(unmanaged, { MoveTemp(wrapper), false });
				CaptureConjugate(unmanaged);

				return { unmanaged };
			}

			return {};
		}

		ConjugateWrapperType* FindConjugateWrapper(const void* unmanaged)
		{
			const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
			return rec ? rec->Conjugate.Get() : nullptr;
		}
		
		TImpl& AsImpl() { return static_cast<TImpl&>(*this); }

	protected:
		IZMasterAssemblyLoadContext& Alc;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<ConjugateWrapperType> Conjugate;
			bool bBlack;
		};
		
		struct FZRedFrame
		{
			TArray<void*> CapturedConjugates;
		};

#if ZSHARP_TREATS_BLACK_CONJUGATE_AS_GC_OBJECT
	private:
		virtual void AddReferencedObjects(FReferenceCollector& collector) override
		{
			if constexpr (IsGCObject)
			{
				for (auto& pair : ConjugateMap)
				{
					if (pair.Value.bBlack)
					{
						pair.Value.Conjugate->AddReferencedObjects(collector);
					}
				}
			}
		}
#endif
		
	private:
		virtual void Release() override
		{
			TArray<void*> reds;
			for (const auto& pair : ConjugateMap)
			{
				if (!pair.Value.bBlack)
				{
					reds.Emplace(pair.Key);
				}
			}

			for (const auto& red : reds)
			{
				ReleaseConjugate_Red(red);
			}
		}
		
		virtual void* BuildConjugate(void* userdata) override
		{
			return BuildConjugate_Black(userdata);
		}
		
		virtual void ReleaseConjugate(void* unmanaged) override
		{
			ReleaseConjugate_Black(unmanaged);
		}
		
		virtual void PushRedFrame() override
		{
			RedStack.Push({});
		}
		
		virtual void PopRedFrame() override
		{
			FZRedFrame frame = RedStack.Pop();
			for (const auto& conjugate : frame.CapturedConjugates)
			{
				ReleaseConjugate_Red(conjugate);
			}
		}

	private:
		void* BuildConjugate_Black(void* userdata)
		{
			auto wrapper = TUniquePtr<ConjugateWrapperType> { ThisClass::BuildConjugateWrapper(userdata) };
			void* unmanaged = wrapper->GetUnderlyingInstance();
			if (unmanaged)
			{
#if ZSHARP_TREATS_BLACK_CONJUGATE_AS_GC_OBJECT
				if constexpr (IsGCObject)
				{
					Referencer.Activate();
				}
#endif
				
				ConjugateMap.Emplace(unmanaged, { MoveTemp(wrapper), true });
			}
			return unmanaged;
		}
		
		void ReleaseConjugate_Black(void* unmanaged)
		{
			ensure(ConjugateMap.Remove(unmanaged));
		}

		void ReleaseConjugate_Red(void* unmanaged)
		{
			if (ensure(ConjugateMap.Remove(unmanaged)))
			{
				Alc.ReleaseConjugate(unmanaged);
			}
		}

		void CaptureConjugate(void* unmanaged)
		{
			check(!RedStack.IsEmpty());
			RedStack.Top().CapturedConjugates.Emplace(unmanaged);
		}

	private:
		TMap<void*, FZConjugateRec> ConjugateMap;
		TArray<FZRedFrame> RedStack;
		ZConjugateRegistryBase_Private::FZLazyReferencer Referencer;
		
	};
}


