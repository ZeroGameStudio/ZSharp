// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZGCHandle.h"

namespace ZSharp
{
	class IZType;
	
	class ZSHARPCLR_API IZAssembly : public IZGCHandle
	{
	public:
		virtual const FString& GetName() const = 0;
	public:
		virtual const IZType* GetType(const FString& name) const = 0;
	};
}
