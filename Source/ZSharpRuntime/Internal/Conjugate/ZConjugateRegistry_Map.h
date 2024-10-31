// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Map : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_Map;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveScriptMap>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveScriptMap> Map;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_Map(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const FProperty* keyProperty, const FProperty* valueProperty) { return Conjugate(keyProperty, valueProperty, [](const FZSelfDescriptiveScriptMap&){}); }
		FZConjugateHandle Conjugate(const FProperty* keyProperty, const FProperty* valueProperty, TFunctionRef<void(const FZSelfDescriptiveScriptMap&)> initialize);
		FZConjugateHandle Conjugate(const FProperty* keyProperty, const FProperty* valueProperty, const FScriptMap* unmanaged);
		FZSelfDescriptiveScriptMap* Conjugate(FZConjugateHandle handle) const;

	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* keyProperty, const FProperty* valueProperty) const;
		
	private:
		TMap<void*, FZConjugateRec> ConjugateMap;
	
	};
}


