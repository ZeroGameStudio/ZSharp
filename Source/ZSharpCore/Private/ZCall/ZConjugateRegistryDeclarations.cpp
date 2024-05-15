// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistryDeclarations.h"

#include "ZSharpCoreLogChannels.h"

namespace ZSharp::FZConjugateRegistryDeclarations_Private
{
	static TMap<uint16, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>> DeclarationMap;
}

void ZSharp::FZConjugateRegistryDeclarations::Declare(uint16 id, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&& factory)
{
	if (FZConjugateRegistryDeclarations_Private::DeclarationMap.Contains(id))
	{
		UE_LOG(LogZSharpCore, Fatal, TEXT("Ambiguous conjugate registry id [%d]"), id);
	}
	
	FZConjugateRegistryDeclarations_Private::DeclarationMap.Emplace(id, MoveTemp(factory));
}

void ZSharp::FZConjugateRegistryDeclarations::ForeachDeclaration(const TFunctionRef<void(uint16, const TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&)> action)
{
	for (const auto& pair : FZConjugateRegistryDeclarations_Private::DeclarationMap)
	{
		action(pair.Key, pair.Value);
	}
}


