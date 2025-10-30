// Copyright Zero Games. All Rights Reserved.

#include "ZInteropStringArray_Interop.h"

#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZInteropStringArray_Interop_Private
{
	static FZInteropStringArray_Interop::ArrayType& Alloc()
	{
		return *new FZInteropStringArray_Interop::ArrayType{};
	}
	
	static void Set(FZInteropStringArray_Interop::ArrayType& address, int32 index, const TCHAR* value)
	{
		address[index] = FString { value };
	}
	
	static void Add(FZInteropStringArray_Interop::ArrayType& address, const TCHAR* value)
	{
		address.Emplace(FString { value });
	}
	
	static void InsertAt(FZInteropStringArray_Interop::ArrayType& address, int32 index, const TCHAR* value)
	{
		address.Insert(FString { value }, index);
	}
}

ZSharp::FZInteropStringArray_Interop::ArrayType& ZSharp::FZInteropStringArray_Interop::Alloc()
{
	TRY
	{
		return ZInteropStringArray_Interop_Private::Alloc();
	}
	CATCHR(*(ArrayType*)nullptr)
}

void ZSharp::FZInteropStringArray_Interop::Free(ArrayType& address)
{
	GUARD(delete &address);
}

int32 ZSharp::FZInteropStringArray_Interop::Count(ArrayType& address)
{
	TRY
	{
		return address.Num();
	}
	CATCHR(0)
}

const TCHAR* ZSharp::FZInteropStringArray_Interop::Get(ArrayType& address, int32 index)
{
	TRY
	{
		return ::GetData(address[index]);
	}
	CATCHR(nullptr)
}

void ZSharp::FZInteropStringArray_Interop::Set(ArrayType& address, int32 index, const TCHAR* value)
{
	GUARD(ZInteropStringArray_Interop_Private::Set(address, index, value));
}

void ZSharp::FZInteropStringArray_Interop::Add(ArrayType& address, const TCHAR* value)
{
	GUARD(ZInteropStringArray_Interop_Private::Add(address, value));
}

void ZSharp::FZInteropStringArray_Interop::InsertAt(ArrayType& address, int32 index, const TCHAR* value)
{
	GUARD(ZInteropStringArray_Interop_Private::InsertAt(address, index, value));
}

void ZSharp::FZInteropStringArray_Interop::RemoveAt(ArrayType& address, int32 index)
{
	GUARD(address.RemoveAt(index));
}

void ZSharp::FZInteropStringArray_Interop::Clear(ArrayType& address)
{
	GUARD(address.Empty());
}


