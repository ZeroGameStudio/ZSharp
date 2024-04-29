// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/IZCallResolver.h"

namespace ZSharp
{
	class FZCallResolver_UFunction : public IZCallResolver
	{
		
	public:
		virtual IZCallDispatcher* Resolve(const FString& name) const override;
		
	};
}


