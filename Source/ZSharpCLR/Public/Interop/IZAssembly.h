// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZObject.h"
#include "IZCustomAttributeProvider.h"

namespace ZSharp
{
	class IZType;
	
	class ZSHARPCLR_API IZAssembly : public IZObject, public IZCustomAttributeProvider
	{
	public:
		virtual IZType* GetType(const FString& name) const = 0;
	};
}
