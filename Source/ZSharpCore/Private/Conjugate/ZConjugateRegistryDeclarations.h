// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	class IZConjugateRegistry;
	
	struct FZConjugateRegistryDeclarations
	{
		static void Declare(uint16 id, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&& factory);
		static void ForeachDeclaration(const TFunctionRef<void(uint16, const TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&)> action);
	};
}


