// Copyright Zero Games. All Rights Reserved.

#include "ZCall/ZCallHandle.h"

ZSharp::FZCallHandle ZSharp::FZCallHandle::Alloc()
{
	static int64 GCurrentHandle = 0;

	FZCallHandle handle;
	handle.Handle = ++GCurrentHandle;
	
	return handle;
}


