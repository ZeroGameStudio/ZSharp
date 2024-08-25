// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	class ZSHARPCORE_API IZSlimAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual FString GetName() const = 0;
	};
}


