// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "ZCall/ZCallHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	struct FZInteropRuntimeTypeUri;

	struct FZMasterAssemblyLoadContext_Interop
	{
		static EZCallErrorCode ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer);
		static FZCallHandle GetZCallHandle_Black(const TCHAR* name);
		static void* BuildConjugate_Black(uint16 registryId, void* userdata);
		static void ReleaseConjugate_Black(uint16 registryId, void* unmanaged);

		inline static int32(*GUnload)() = nullptr;
		inline static EZLoadAssemblyErrorCode(*GLoadAssembly)(const TCHAR*, void*) = nullptr;
		inline static EZInvokeMethodErrorCode(*GInvokeMethod)(const TCHAR*, const TCHAR*, const TCHAR*, void*) = nullptr;
		inline static FZRuntimeTypeHandle(*GGetType)(FZInteropRuntimeTypeUri uri) = nullptr;

		inline static EZCallErrorCode(*GZCall_Red)(FZCallHandle handle, FZCallBuffer* buffer, FString* fatalMessage);
		inline static FZCallHandle(*GGetZCallHandle_Red)(const TCHAR*);
		inline static void*(*GBuildConjugate_Red)(void* unmanaged, FZRuntimeTypeHandle type);
		inline static void (*GReleaseConjugate_Red)(void* unmanaged);
	};
}


