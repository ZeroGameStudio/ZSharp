// Copyright Zero Games. All Rights Reserved.

#include "ALC/ZRedFrameScope.h"

#include "CLR/ZSharpClr.h"

ZSharp::FZRedFrameScope::FZRedFrameScope()
{
	IZMasterAssemblyLoadContext* alc = FZSharpClr::Get().GetMasterAlc();
	CapturedAlc = alc;
	if (alc)
	{
		alc->PushRedFrame();
	}
}

ZSharp::FZRedFrameScope::~FZRedFrameScope()
{
	IZMasterAssemblyLoadContext* alc = FZSharpClr::Get().GetMasterAlc();
	if (ensureAlways(alc == CapturedAlc))
	{
		alc->PopRedFrame();
	}
}


