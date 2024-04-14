// Copyright Zero Games. All Rights Reserved.


#include "Interop/ZCallHandle.h"

ZSharp::FZCallHandle ZSharp::FZCallHandle::Alloc()
{
	static uint64 GCurrentHandle = 0;

	FZCallHandle handle;
	handle.Handle = ++GCurrentHandle;

	return handle;
}


