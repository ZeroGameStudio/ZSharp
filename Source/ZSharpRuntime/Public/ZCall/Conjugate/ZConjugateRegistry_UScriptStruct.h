// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Trait/ZIsUScriptStruct.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_UScriptStruct : public FZConjugateRegistryBase
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_UScriptStruct;

	public:
		static constexpr uint16 RegistryId = GUStructConjugateRegistryId;

	private:
		struct FZConjugateRec
		{
			TStrongObjectPtr<UScriptStruct> ScriptStruct;
			void* Instance;
			bool bOwning;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_UScriptStruct(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const UScriptStruct* scriptStruct, const void* unmanaged, bool owning);
		
		template <typename T>
		requires TZIsUScriptStruct_V<T> || std::is_same_v<T, void>
		T* Conjugate(FZConjugateHandle handle)
		{
			const FZConjugateRec* rec = Conjugate(handle);
			if (!rec)
			{
				return nullptr;
			}
			
			if constexpr (std::is_same_v<T, void>)
			{
				return rec->Instance;
			}
			else
			{
				check(rec->ScriptStruct.Get() == TBaseStructure<T>::Get());
				return static_cast<T*>(rec->Instance);
			}
		}

	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		const FZConjugateRec* Conjugate(FZConjugateHandle handle);
		FZRuntimeTypeHandle GetManagedType(const UScriptStruct* scriptStruct) const;
		
	private:
		TMap<void*, FZConjugateRec> ConjugateMap;
		
	
	};
}

