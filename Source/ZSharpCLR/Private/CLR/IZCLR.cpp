// Copyright Zero Games. All Rights Reserved.

#include "CLR/IZCLR.h"

#include "CLRImplType.h"
#include "ZGenericCLR.h"

ZSharp::IZCLR& ZSharp::IZCLR::Get()
{
	static FZCLRImplType GSingleton;

	return GSingleton;
}


