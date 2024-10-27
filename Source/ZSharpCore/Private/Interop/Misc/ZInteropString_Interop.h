// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZInteropString_Interop
	{
		static FString& Alloc(const TCHAR* data);
		static void Free(FString& address);
		static const TCHAR* GetData(FString& address);
		static void SetData(FString& address, const TCHAR* data);
	};
}


