// Copyright Zero Games. All Rights Reserved.

#include "Static/ZStaticallyExportedEnum.h"

#include "ZExportedTypeRegistry.h"

bool ZSharp::ZStaticallyExportedEnum_Private::RegisterEnum(IZExportedEnum* enm)
{
	return FZExportedTypeRegistry::Get().RegisterEnum(enm);
}


