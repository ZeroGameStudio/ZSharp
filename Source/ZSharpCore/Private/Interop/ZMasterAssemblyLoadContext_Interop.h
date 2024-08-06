// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZRuntimeTypeHandle.h"
#include "ZCall/ZCallHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	struct FZRuntimeTypeLocator;
	
	struct FZMasterAssemblyLoadContext_Interop
	{
		static int32 ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer);
		static FZCallHandle GetZCallHandle_Black(const TCHAR* name);
		static void* BuildConjugate_Black(uint16 registryId, void* userdata);
		static void ReleaseConjugate_Black(uint16 registryId, void* unmanaged);

		inline static uint8(*GTick)(float) = nullptr;
		inline static int32(*GUnload)() = nullptr;
		inline static int32(*GLoadAssembly)(const uint8*, int32, void*) = nullptr;
		inline static FZRuntimeTypeHandle(*GGetType)(const FZRuntimeTypeLocator&) = nullptr;

		inline static int32(*GZCall_Red)(FZCallHandle handle, FZCallBuffer* buffer);
		inline static FZCallHandle(*GGetZCallHandle_Red)(const TCHAR*);
		inline static void*(*GBuildConjugate_Red)(void* unmanaged, FZRuntimeTypeHandle type);
		inline static void (*GReleaseConjugate_Red)(void* unmanaged);
	};
}


