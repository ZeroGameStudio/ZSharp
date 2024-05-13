// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZDeclareConjugateRegistry.h"

#include "ZConjugateRegistryDeclarations.h"

void ZSharp::ZDeclareConjugateRegistry_Private::DeclareConjugateRegistry(uint16 id, IZConjugateRegistry*(*factory)(IZMasterAssemblyLoadContext&))
{
	FZConjugateRegistryDeclarations::Declare(id, factory);
}


