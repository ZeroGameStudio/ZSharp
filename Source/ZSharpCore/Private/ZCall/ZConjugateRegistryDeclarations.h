// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	class IZConjugateRegistry;
	
	struct FZConjugateRegistryDeclarations
	{
		static void Declare(uint16 id, IZConjugateRegistry*(*factory)(IZMasterAssemblyLoadContext&));
		static void ForeachDeclaration(const TFunctionRef<void(uint16, IZConjugateRegistry*(*)(IZMasterAssemblyLoadContext&))> action);
	};
}


