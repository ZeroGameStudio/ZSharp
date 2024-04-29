// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZCallHandle.h"

ZSharp::FZCallHandle ZSharp::AllocateZCallHandle()
{
	static uint64 GCurrentHandle = 0;

	FZCallHandle handle;
	handle.Handle = ++GCurrentHandle;
	
	return handle;
}
