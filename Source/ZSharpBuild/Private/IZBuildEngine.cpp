// Copyright Zero Games. All Rights Reserved.


#include "IZBuildEngine.h"

#include "ZBuildEngine.h"

ZSharp::IZBuildEngine& ZSharp::IZBuildEngine::Get()
{
	return FZBuildEngine::Get();
}


