// Copyright Zero Games. All Rights Reserved.

#include "ZInteropStringArray_Interop.h"

ZSharp::FZInteropStringArray_Interop::ArrayType& ZSharp::FZInteropStringArray_Interop::Alloc()
{
	return *new ArrayType{};
}

void ZSharp::FZInteropStringArray_Interop::Free(ArrayType& address)
{
	delete &address;
}

int32 ZSharp::FZInteropStringArray_Interop::Count(ArrayType& address)
{
	return address.Num();
}

const TCHAR* ZSharp::FZInteropStringArray_Interop::Get(ArrayType& address, int32 index)
{
	return GetData(address[index]);
}

void ZSharp::FZInteropStringArray_Interop::Set(ArrayType& address, int32 index, const TCHAR* value)
{
	address[index] = FString { value };
}

void ZSharp::FZInteropStringArray_Interop::Add(ArrayType& address, const TCHAR* value)
{
	address.Emplace(FString { value });
}

void ZSharp::FZInteropStringArray_Interop::InsertAt(ArrayType& address, int32 index, const TCHAR* value)
{
	address.Insert(FString { value }, index);
}

void ZSharp::FZInteropStringArray_Interop::RemoveAt(ArrayType& address, int32 index)
{
	address.RemoveAt(index);
}

void ZSharp::FZInteropStringArray_Interop::Clear(ArrayType& address)
{
	address.Empty();
}


