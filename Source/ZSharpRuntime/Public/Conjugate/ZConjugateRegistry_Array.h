// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "ZCall/Container/ZSelfDescriptiveScriptArray.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Array : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_Array;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveScriptArray>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveScriptArray> Array;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_Array(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const FProperty* elementPropertyProto) { return Conjugate(elementPropertyProto, [](const FZSelfDescriptiveScriptArray&){}); }
		FZConjugateHandle Conjugate(const FProperty* elementPropertyProto, TFunctionRef<void(const FZSelfDescriptiveScriptArray&)> initialize);
		FZConjugateHandle Conjugate(const FProperty* elementProperty, const FScriptArray* unmanaged);
		FZSelfDescriptiveScriptArray* Conjugate(FZConjugateHandle handle) const;

	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;
		
	private:
		TMap<void*, FZConjugateRec> ConjugateMap;
	
	};
}


