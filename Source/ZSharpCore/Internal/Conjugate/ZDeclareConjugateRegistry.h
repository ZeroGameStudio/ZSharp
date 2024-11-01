// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/IZConjugateRegistry.h"
#include "Concept/CZConjugateRegistryImpl.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;

	namespace ZDeclareConjugateRegistry_Private
	{
		ZSHARPCORE_API void DeclareConjugateRegistry(uint16 id, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&& factory);
	}
	
	template <CZConjugateRegistryImpl T>
	struct TZDeclareConjugateRegistry
	{
		TZDeclareConjugateRegistry()
		{
			ZDeclareConjugateRegistry_Private::DeclareConjugateRegistry(T::RegistryId, [](IZMasterAssemblyLoadContext& alc) -> IZConjugateRegistry* { return new T { alc }; });
		}
	};
}


