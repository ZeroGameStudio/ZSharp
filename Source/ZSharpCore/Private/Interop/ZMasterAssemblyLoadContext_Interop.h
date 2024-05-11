// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"
#include "ZCall/ZCallHandle.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	struct FZMasterAssemblyLoadContext_Interop
	{
		static int32 ZCallByHandle(FZCallHandle handle, FZCallBuffer* buffer);
		static int32 ZCallByName(const TCHAR* name, FZCallBuffer* buffer, FZCallHandle* outHandle);
		static void ZCallByHandle_AnyThread(FZCallHandle handle, FZCallBuffer* buffer, int32 numSlots);
		static FZCallHandle GetZCallHandle(const TCHAR* name);
		
		inline static int32(*GUnload)() = nullptr;
		inline static FZGCHandle(*GLoadAssembly)(const uint8*, int32, void*) = nullptr;
		inline static int32(*GReleaseConjugate)(FZConjugateHandle) = nullptr;
	};
}


