// Copyright Zero Games. All Rights Reserved.


#include "IZExportedTypeRegistry.h"

#include "ZExportedTypeRegistry.h"

ZSharp::IZExportedTypeRegistry& ZSharp::IZExportedTypeRegistry::Get()
{
	return FZExportedTypeRegistry::Get();
}

bool ZSharp::IZExportedTypeRegistry_Private::RegisterClass(IZExportedClass* cls)
{
	return FZExportedTypeRegistry::Get().RegisterClass(cls);
}

bool ZSharp::IZExportedTypeRegistry_Private::RegisterEnum(IZExportedEnum* enm)
{
	return FZExportedTypeRegistry::Get().RegisterEnum(enm);
}


