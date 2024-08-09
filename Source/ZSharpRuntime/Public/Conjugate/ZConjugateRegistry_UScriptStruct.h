// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Wrapper/ZSelfDescriptiveScriptStruct.h"
#include "ZConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_UScriptStruct : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_UScriptStruct;

	public:
		static constexpr uint16 RegistryId = GUStructConjugateRegistryId;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveScriptStruct> ScriptStruct;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_UScriptStruct(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const UScriptStruct* scriptStruct) { return Conjugate(scriptStruct, static_cast<TFunctionRef<void(const FZSelfDescriptiveScriptStruct&)>>([](const FZSelfDescriptiveScriptStruct&){})); }
		FZConjugateHandle Conjugate(const UScriptStruct* scriptStruct, TFunctionRef<void(const FZSelfDescriptiveScriptStruct&)> initialize);
		FZConjugateHandle Conjugate(const UScriptStruct* scriptStruct, const void* unmanaged);
		const FZSelfDescriptiveScriptStruct* Conjugate(FZConjugateHandle handle) const;

	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		FZRuntimeTypeHandle GetManagedType(const UScriptStruct* scriptStruct) const;
		
	private:
		TMap<void*, FZConjugateRec> ConjugateMap;
	
	};
}

