// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZConjugateRegistry.h"
#include "CZConjugateRegistryImpl.h"

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


