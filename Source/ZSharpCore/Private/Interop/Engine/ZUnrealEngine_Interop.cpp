// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealEngine_Interop.h"

uint8 ZSharp::FZUnrealEngine_Interop::IsInGameThread()
{
	return !!::IsInGameThread();
}


