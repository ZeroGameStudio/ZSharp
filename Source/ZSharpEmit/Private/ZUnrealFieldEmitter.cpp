// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealFieldEmitter.h"

ZSharp::FZUnrealFieldEmitter& ZSharp::FZUnrealFieldEmitter::Get()
{
	static FZUnrealFieldEmitter GSingleton;

	return GSingleton;
}


