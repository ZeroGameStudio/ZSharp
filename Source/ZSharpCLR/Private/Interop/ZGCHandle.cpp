// Copyright Zero Games. All Rights Reserved.

#include "Interop/ZGCHandle.h"

#include "ZGCHandle_Interop.h"

void ZSharp::Free(FZGCHandle handle)
{
	FZGCHandle_Interop::GFree(handle);
}


