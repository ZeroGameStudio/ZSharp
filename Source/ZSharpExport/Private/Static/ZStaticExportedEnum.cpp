// Copyright Zero Games. All Rights Reserved.


#include "Static/ZStaticExportedEnum.h"

#include "ZExportedTypeRegistry.h"

bool ZSharp::ZStaticExportedEnum_Private::RegisterEnum(IZExportedEnum* enm)
{
	return FZExportedTypeRegistry::Get().RegisterEnum(enm);
}


