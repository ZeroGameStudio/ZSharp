// Copyright Zero Games. All Rights Reserved.

#include "ALC/ZRedFrameScope.h"

#include "CLR/ZSharpClr.h"

ZSharp::FZRedFrameScope::FZRedFrameScope()
{
	FZSharpClr::Get().GetMasterAlc()->PushRedFrame();
}

ZSharp::FZRedFrameScope::~FZRedFrameScope()
{
	FZSharpClr::Get().GetMasterAlc()->PopRedFrame();
}


