// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Map : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Map, FZSelfDescriptiveScriptMap>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Map(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveScriptMap* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const TPair<const FProperty*, const FProperty*>* descriptor, const FZSelfDescriptiveScriptMap* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const TPair<const FProperty*, const FProperty*>* descriptor) const;
		
	};
}


