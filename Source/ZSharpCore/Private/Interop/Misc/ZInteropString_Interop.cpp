// Copyright Zero Games. All Rights Reserved.

#include "ZInteropString_Interop.h"

FString& ZSharp::FZInteropString_Interop::Alloc(const TCHAR* data)
{
	return *new FString { data };
}

void ZSharp::FZInteropString_Interop::Free(FString& address)
{
	delete &address;
}

const TCHAR* ZSharp::FZInteropString_Interop::GetData(FString& address)
{
	return *address;
}

void ZSharp::FZInteropString_Interop::SetData(FString& address, const TCHAR* data)
{
	address = data;
}


