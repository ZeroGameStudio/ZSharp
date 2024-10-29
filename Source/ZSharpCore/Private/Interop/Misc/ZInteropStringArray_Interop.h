// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZInteropStringArray_Interop
	{
		using ArrayType = TArray<FString>;
		
		static ArrayType& Alloc();
		static void Free(ArrayType& address);
		static int32 Count(ArrayType& address);
		static const TCHAR* Get(ArrayType& address, int32 index);
		static void Set(ArrayType& address, int32 index, const TCHAR* value);
		static void Add(ArrayType& address, const TCHAR* value);
		static void InsertAt(ArrayType& address, int32 index, const TCHAR* value);
		static void RemoveAt(ArrayType& address, int32 index);
		static void Clear(ArrayType& address);
	};
}


