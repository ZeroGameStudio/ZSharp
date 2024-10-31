// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Reflection/ZReflectionHelper.h"

namespace ZSharp
{
	template <typename T>
	class TZObjectWrapperConjugateRegistryBase : public FZConjugateRegistryBase, public FNoncopyable
	{

		using WrapperType = typename T::WrapperType;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<T>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<T> Wrapper;
			bool bBlack;
		};

	public:
		explicit TZObjectWrapperConjugateRegistryBase(IZMasterAssemblyLoadContext& alc) : FZConjugateRegistryBase(alc){}

	public:
		FZConjugateHandle Conjugate(const UClass* descriptor) { return Conjugate(descriptor, [](const T&){}); }
		FZConjugateHandle Conjugate(const UClass* descriptor, TFunctionRef<void(const T&)> initialize)
		{
			const FZRuntimeTypeHandle type = GetManagedType(descriptor);
			auto sdow = new T { descriptor };
			initialize(*sdow);
			void* unmanaged = sdow->GetUnderlyingInstance();
			if (Alc.BuildConjugate(unmanaged, type))
			{
				ConjugateMap.Emplace(unmanaged, { TUniquePtr<T>(sdow), false });
				CaptureConjugate(unmanaged);

				return { unmanaged };
			}

			return {};
		}
		
		FZConjugateHandle Conjugate(const UClass* descriptor, const WrapperType* unmanaged)
		{
			auto mutableUnmanaged = (WrapperType*)unmanaged;
			if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
			{
				check(rec->Wrapper->GetDescriptor() == descriptor);
				return { mutableUnmanaged };
			}

			const FZRuntimeTypeHandle type = GetManagedType(descriptor);
			if (Alc.BuildConjugate(mutableUnmanaged, type))
			{
				TUniquePtr<T> pSdow = MakeUnique<T>(descriptor, mutableUnmanaged);
				ConjugateMap.Emplace(mutableUnmanaged, { MoveTemp(pSdow), false });
				CaptureConjugate(mutableUnmanaged);

				return { mutableUnmanaged };
			}

			return {};
		}
		
		T* Conjugate(FZConjugateHandle handle) const
		{
			const void* unmanaged = handle.Handle;
			const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
			return rec ? rec->Wrapper.Get() : nullptr;
		}

	private:
		virtual void* BuildConjugate(void* userdata) override
		{
			const auto descriptor = static_cast<UClass*>(userdata);
			auto pSdow = MakeUnique<T>(descriptor);
			void* unmanaged = pSdow->GetUnderlyingInstance();
			ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdow), true });
			return unmanaged;
		}
		
		virtual void ReleaseConjugate(void* unmanaged) override
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
		
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override
		{
			for (const auto& pair : ConjugateMap)
			{
				outConjugates.Emplace(pair.Key);
			}
		}

	private:
		FZRuntimeTypeHandle GetManagedType(const UClass* cls) const
		{
			FZRuntimeTypeLocatorWrapper locator;
			locator.AssemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME;
			locator.TypeName = FString::Printf(TEXT("%s.CoreUObject.%s`1"), *locator.AssemblyName, *T::GetExportTypeName());
			FZRuntimeTypeLocatorWrapper& inner = locator.TypeParameters.Emplace_GetRef();
			FZReflectionHelper::GetUFieldRuntimeTypeLocator(cls, inner);
	
			return Alc.GetType(locator);
		}
		
	private:
		TMap<void*, FZConjugateRec> ConjugateMap;
		
	};
}
