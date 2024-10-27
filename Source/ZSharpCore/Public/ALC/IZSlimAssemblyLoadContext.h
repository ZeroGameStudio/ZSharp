// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	class IZSlimAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual FString GetName() const = 0;
	};
}


