// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	class ZSHARPCORE_API IZDefaultAssemblyLoadContext : public IZAssemblyLoadContext
	{
		// These operations are banned on default ALC.
		virtual FZGCHandle GetGCHandle() const override final { checkNoEntry(); return {}; }
		virtual void Unload() override final { checkNoEntry(); }
	};
}


