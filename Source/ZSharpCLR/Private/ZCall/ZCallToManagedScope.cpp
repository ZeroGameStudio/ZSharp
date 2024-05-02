// Copyright Zero Games. All Rights Reserved.


#include "ZCallToManagedScope.h"

#include "ALC/ZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpCLR.h"

ZSharp::FZCallToManagedScope::FZCallToManagedScope()
{
	StaticCast<FZMasterAssemblyLoadContext*>(IZSharpCLR::Get().GetMasterAlc())->NotifyPreZCallToManaged();
}

ZSharp::FZCallToManagedScope::~FZCallToManagedScope()
{
	StaticCast<FZMasterAssemblyLoadContext*>(IZSharpCLR::Get().GetMasterAlc())->NotifyPostZCallToManaged();
}


