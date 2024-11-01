// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	class IZDefaultAssemblyLoadContext : public IZAssemblyLoadContext
	{
		virtual void Unload() override final { checkNoEntry(); }
	};
}


