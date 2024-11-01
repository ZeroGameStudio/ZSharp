// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	/**
	 * Represents System.Runtime.Loader.AssemblyLoadContext.Default.
	 * 
	 * [Library Interface] - you should just consume this and have no reason to implement this.
	 */
	class IZDefaultAssemblyLoadContext : public IZAssemblyLoadContext
	{
		/** Unloading Default ALC is not allowed. */
		virtual void Unload() override final { checkNoEntry(); }
	};
}


