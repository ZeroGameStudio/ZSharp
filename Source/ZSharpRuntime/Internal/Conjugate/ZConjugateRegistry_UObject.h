// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/IZConjugateRegistry.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Conjugate/IZUObjectConjugateController.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_UObject : public IZConjugateRegistry, public FNoncopyable
	{

		using ThisClass = FZConjugateRegistry_UObject;

	public:
		static constexpr uint16 RegistryId = GUObjectConjugateRegistryId;

	public:
		explicit FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc);

	public:
		UObject* ConjugateUnsafe(FZConjugateHandle handle) const;
		UObject* Conjugate(FZConjugateHandle handle) const;
		FZConjugateHandle Conjugate(const UObjectBase* unmanaged);

		template <typename T>
		requires TZIsUClass_V<T>
		T* ConjugateUnsafe(FZConjugateHandle handle) const
		{
			return Cast<T>(ConjugateUnsafe(handle));
		}
		
		template <typename T>
		requires TZIsUClass_V<T>
		T* Conjugate(FZConjugateHandle handle) const
		{
			return Cast<T>(Conjugate(handle));
		}

		template <typename T>
		requires TZIsUClass_V<T>
		T* ConjugateUnsafeChecked(FZConjugateHandle handle) const
		{
			return CastChecked<T>(ConjugateUnsafe(handle));
		}
		
		template <typename T>
		requires TZIsUClass_V<T>
		T* ConjugateChecked(FZConjugateHandle handle) const
		{
			return CastChecked<T>(Conjugate(handle));
		}

	public:
		void RegisterController(IZUObjectConjugateController* controller);

	private:
		virtual void Release() override;
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;

	private:
		void ReleaseConjugate_Red(void* unmanaged);
		bool CanBuildConjugate(UObject* unmanaged) const;
		void NotifyConjugated(UObject* unmanaged);
		void NotifyLifecycleExpired(UObject* unmanaged);

	private:
		FZRuntimeTypeHandle GetManagedType(const UObject* unmanaged) const;

	private:
		IZMasterAssemblyLoadContext& Alc;
		TMap<void*, FObjectKey> ConjugateMap;

		TArray<TUniquePtr<IZUObjectConjugateController>> Controllers;
		
	};
}


