// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Set : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_Set;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveScriptSet>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveScriptSet> Set;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_Set(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const FProperty* elementPropertyProto) { return Conjugate(elementPropertyProto, [](const FZSelfDescriptiveScriptSet&){}); }
		FZConjugateHandle Conjugate(const FProperty* elementPropertyProto, TFunctionRef<void(const FZSelfDescriptiveScriptSet&)> initialize);
		FZConjugateHandle Conjugate(const FProperty* elementProperty, const FScriptSet* unmanaged);
		FZSelfDescriptiveScriptSet* Conjugate(FZConjugateHandle handle) const;

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


