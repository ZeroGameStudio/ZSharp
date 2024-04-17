// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZCallResolver.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	class FZCallResolver_Export : public IZCallResolver
	{

	public:
		static void RegisterFunction(const FString& name, const TFunction<int32(FZCallBuffer*)> function);

	public:
		virtual IZCallDispatcher* Resolve(const FString& name) const override;
		
	};
}


