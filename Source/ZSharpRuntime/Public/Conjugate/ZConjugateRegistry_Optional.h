// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Optional : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_Optional;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveOptional>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveOptional> Optional;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_Optional(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const FProperty* elementPropertyProto) { return Conjugate(elementPropertyProto, static_cast<TFunctionRef<void(const FZSelfDescriptiveOptional&)>>([](const FZSelfDescriptiveOptional&){})); }
		FZConjugateHandle Conjugate(const FProperty* elementPropertyProto, TFunctionRef<void(const FZSelfDescriptiveOptional&)> initialize);
		FZConjugateHandle Conjugate(const FProperty* elementProperty, const void* unmanaged);
		FZSelfDescriptiveOptional* Conjugate(FZConjugateHandle handle) const;

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


