// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZDeclareConjugateRegistry.h"

#include "ZConjugateRegistryDeclarations.h"

void ZSharp::ZDeclareConjugateRegistry_Private::DeclareConjugateRegistry(uint16 id, TUniqueFunction<IZConjugateRegistry*(IZMasterAssemblyLoadContext&)>&& factory)
{
	FZConjugateRegistryDeclarations::Declare(id, MoveTemp(factory));
}


