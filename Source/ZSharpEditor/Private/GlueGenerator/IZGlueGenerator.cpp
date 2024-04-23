// Copyright Zero Games. All Rights Reserved.

#include "GlueGenerator/IZGlueGenerator.h"

#include "ZGlueGenerator.h"

ZSharp::IZGlueGenerator& ZSharp::IZGlueGenerator::Get()
{
	static FZGlueGenerator GSingleton;
	
	return GSingleton;
}


