// Copyright Zero Games. All Rights Reserved.

#include "Interop/ZGCHandle.h"

#include "ZGCHandle_Interop.h"

void ZSharp::FZGCHandle::Free()
{
	FZGCHandle_Interop::GFree(*this);
	Handle = nullptr;
}
