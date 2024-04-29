// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZGCHandle.h"

namespace ZSharp
{
	class IZAssembly;
	
	class ZSHARPCLR_API IZAssemblyLoadContext : public IZGCHandle
	{
	public:
		virtual void Unload() = 0;
	};
}


