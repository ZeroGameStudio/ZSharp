// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZMasterAssemblyLoadContext_Interop
	{
		inline static void(*GUnload)() = nullptr;
		inline static FZGCHandle(*GLoadAssembly)(const uint8*, int32) = nullptr;
	};
}


