// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZMemberInfo.h"
#include "ZCallToManaged.h"

namespace ZSharp
{
	class ZSHARPCLR_API IZMethodInfo : public IZMemberInfo
	{
	public:
		virtual ZCALL_TO_MANAGED_AUTO_BUFFER(Invoke) = 0;
		virtual ZCALL_TO_MANAGED_USER_BUFFER(Invoke) = 0;
		virtual ZCALL_TO_MANAGED_USER_BUFFER_CHECKED(Invoke) = 0;
	};
}
