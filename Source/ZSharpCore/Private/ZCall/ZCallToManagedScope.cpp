// Copyright Zero Games. All Rights Reserved.


#include "ZCallToManagedScope.h"

#include "ALC/ZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"

ZSharp::FZCallToManagedScope::FZCallToManagedScope()
{
	StaticCast<FZMasterAssemblyLoadContext*>(IZSharpClr::Get().GetMasterAlc())->NotifyPreZCallToManaged();
}

ZSharp::FZCallToManagedScope::~FZCallToManagedScope()
{
	StaticCast<FZMasterAssemblyLoadContext*>(IZSharpClr::Get().GetMasterAlc())->NotifyPostZCallToManaged();
}


