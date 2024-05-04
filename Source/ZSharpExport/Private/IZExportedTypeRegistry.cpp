// Copyright Zero Games. All Rights Reserved.


#include "IZExportedTypeRegistry.h"

#include "ZExportedTypeRegistry.h"

ZSharp::IZExportedTypeRegistry& ZSharp::IZExportedTypeRegistry::Get()
{
	static FZExportedTypeRegistry GSingleton;

	return GSingleton;
}


