// Copyright Zero Games. All Rights Reserved.


#include "Emit/IZSharpFieldRegistry.h"

#include "ZSharpFieldRegistry.h"

ZSharp::IZSharpFieldRegistry& ZSharp::IZSharpFieldRegistry::Get()
{
	return FZSharpFieldRegistry::Get();
}


