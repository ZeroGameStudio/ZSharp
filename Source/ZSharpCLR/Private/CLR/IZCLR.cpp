// Copyright Zero Games. All Rights Reserved.

#include "CLR/IZCLR.h"

#include "ZGenericCLR.h"

ZSharp::IZCLR& ZSharp::IZCLR::Get()
{
	using ImplType = FZGenericCLR;

	static ImplType GSingleton;

	return GSingleton;
}


