// Copyright Zero Games. All Rights Reserved.

#include "ZConjugateRegistryDeclarations.h"

#include "ZSharpCoreLogChannels.h"

namespace ZSharp::ZConjugateRegistryDeclarations_Private
{
	static TMap<uint16, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>> DeclarationMap;
}

void ZSharp::FZConjugateRegistryDeclarations::Declare(uint16 id, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&& factory)
{
	if (ZConjugateRegistryDeclarations_Private::DeclarationMap.Contains(id))
	{
		UE_LOG(LogZSharpCore, Fatal, TEXT("Ambiguous conjugate registry id [%d]"), id);
	}
	
	ZConjugateRegistryDeclarations_Private::DeclarationMap.Emplace(id, MoveTemp(factory));
}

void ZSharp::FZConjugateRegistryDeclarations::ForeachDeclaration(const TFunctionRef<void(uint16, const TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&)> action)
{
	for (const auto& pair : ZConjugateRegistryDeclarations_Private::DeclarationMap)
	{
		action(pair.Key, pair.Value);
	}
}


