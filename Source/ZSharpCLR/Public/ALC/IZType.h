// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	class ZSHARPCLR_API IZType : public IZMemberInfo
	{
	public:
		virtual FZConjugateHandle BuildConjugate(void* unmanaged) const = 0;
	};
}
