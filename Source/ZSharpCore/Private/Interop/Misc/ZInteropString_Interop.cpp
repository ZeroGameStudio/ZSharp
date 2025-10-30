// Copyright Zero Games. All Rights Reserved.

#include "ZInteropString_Interop.h"

#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZInteropString_Interop_Private
{
	static FString& Alloc(const TCHAR* data)
	{
		return *new FString { data };
	}
}

FString& ZSharp::FZInteropString_Interop::Alloc(const TCHAR* data)
{
	TRY
	{
		return ZInteropString_Interop_Private::Alloc(data);
	}
	CATCHR(*(FString*)nullptr)
}

void ZSharp::FZInteropString_Interop::Free(FString& address)
{
	GUARD(delete &address);
}

const TCHAR* ZSharp::FZInteropString_Interop::GetData(FString& address)
{
	TRY
	{
		return *address;
	}
	CATCHR(nullptr)
}

void ZSharp::FZInteropString_Interop::SetData(FString& address, const TCHAR* data)
{
	GUARD(address = data);
}


