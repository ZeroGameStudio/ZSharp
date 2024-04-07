// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZObject.h"

namespace ZSharp
{
	class IZAssembly;
	
	class ZSHARPCLR_API IZAssemblyLoadContext : public IZObject
	{
	public:
		virtual void Unload() = 0;
	};
}


