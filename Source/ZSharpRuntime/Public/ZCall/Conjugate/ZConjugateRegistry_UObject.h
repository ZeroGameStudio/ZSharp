// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/Conjugate/ZConjugateRegistryBase.h"

namespace ZSharp
{
	namespace ZConjugateRegistry_UObject_Private
	{
		struct FZConjugateRec
		{
			FObjectKey TypedUnmanaged;
		};
	}

	class ZSHARPRUNTIME_API FZConjugateRegistry_UObject : public TZConjugateRegistryBase<FZConjugateRegistry_UObject, UObject, ZConjugateRegistry_UObject_Private::FZConjugateRec>
	{

		friend class Super;

	public:
		explicit FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc);
		virtual ~FZConjugateRegistry_UObject() override;

	public:
		ConjugateType* Conjugate(FZConjugateHandle handle) const { return Super::Conjugate(handle); }
		FZConjugateHandle Conjugate(const UObjectBase* unmanaged) { return Conjugate(static_cast<const UObject*>(unmanaged), false); }

	private:
		FZRuntimeTypeHandle GetManagedType(const ConjugateType* unmanaged) const;
		ConjugateType* GetUnmanaged(const RecordType* rec) const;
		RecordType BuildRedConjugateRec(ConjugateType* unmanaged, bool owning);
		ConjugateType* BuildBlackConjugateRec();
		void InternalReleaseConjugate(void* unmanaged, const RecordType* rec);
		
	private:
		// Hide base function
		FZConjugateHandle Conjugate(const ConjugateType* unmanaged, bool owning) { return Super::Conjugate(unmanaged, owning); }
	
	private:
		virtual void* BuildConjugate() override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void PopRedFrame() override;

	private:
		void HandleGarbageCollectComplete();

	private:
		FDelegateHandle GCDelegate;
		
	};
}


