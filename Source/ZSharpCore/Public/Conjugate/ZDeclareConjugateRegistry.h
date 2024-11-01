// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZConjugateRegistry.h"
#include "Concept/CZConjugateRegistryImpl.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;

	namespace ZDeclareConjugateRegistry_Private
	{
		ZSHARPCORE_API void DeclareConjugateRegistry(uint16 id, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&& factory);
	}

	/**
	 * Declares a conjugate registry to automatically register to Master ALC.
	 * Generally you don't need to care about this
	 * because Z# has implemented necessary conjugates for interacting with Unreal Engine.
	 * 
	 * @tparam T Type of the conjugate registry.
	 */
	template <CZConjugateRegistryImpl T>
	struct TZDeclareConjugateRegistry
	{
		TZDeclareConjugateRegistry()
		{
			ZDeclareConjugateRegistry_Private::DeclareConjugateRegistry(T::RegistryId, [](IZMasterAssemblyLoadContext& alc) -> IZConjugateRegistry* { return new T { alc }; });
		}
	};
}


