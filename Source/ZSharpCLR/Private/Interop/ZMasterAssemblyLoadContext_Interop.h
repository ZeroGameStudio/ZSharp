// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"
#include "Interop/ZCallHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	struct FZMasterAssemblyLoadContext_Interop
	{
		static int32 ZCallByHandle(FZCallHandle handle, FZCallBuffer* buffer);
		static int32 ZCallByName(const TCHAR* name, FZCallBuffer* buffer, FZCallHandle* outHandle, uint8 bResolve);
		static FZCallHandle GetZCallHandle(const TCHAR* name);
		
		inline static void(*GUnload)() = nullptr;
		inline static FZGCHandle(*GLoadAssembly)(const uint8*, int32) = nullptr;
	};
}


