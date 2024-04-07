// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssembly;
	class IZType;
	
	class ZSHARPCLR_API IZMasterAssemblyLoadContext : public IZAssemblyLoadContext
	{
	public:
		virtual IZAssembly* LoadAssembly(const FString& name, TArray<uint8> buffer) = 0;
		virtual IZAssembly* GetAssembly(const FString& name) const = 0;
	};
}


