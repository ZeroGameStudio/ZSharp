// Copyright Zero Games. All Rights Reserved.


#if ZSHARP_WITH_CORECLR

#include "CLR/CoreCLR.h"

ZSharp::FCoreCLR& ZSharp::FCoreCLR::Get()
{
	static FCoreCLR Singleton;

	return Singleton;
}

#endif


