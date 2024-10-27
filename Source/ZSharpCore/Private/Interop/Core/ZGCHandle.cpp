// Copyright Zero Games. All Rights Reserved.

#include "Interop/ZGCHandle.h"

#include "ZGCHandle_Interop.h"

void ZSharp::FZGCHandle::Free()
{
	if (!Handle)
	{
		return;
	}
	
	FZGCHandle_Interop::GFree(*this);
	Handle = nullptr;
}


