// Copyright Zero Games. All Rights Reserved.

#include "CLR/IZSharpCLR.h"

#include "ZSharpCLR.h"
#include "ZGenericCLR.h"

ZSharp::IZSharpCLR& ZSharp::IZSharpCLR::Get()
{
	static FZSharpCLR GSingleton;

	return GSingleton;
}


