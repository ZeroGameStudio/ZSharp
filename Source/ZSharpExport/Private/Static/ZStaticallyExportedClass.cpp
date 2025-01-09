// Copyright Zero Games. All Rights Reserved.

#include "Static/ZStaticallyExportedClass.h"

#include "ZExportedTypeRegistry.h"

bool ZSharp::ZStaticallyExportedClass_Private::RegisterClass(IZExportedClass* cls)
{
	return FZExportedTypeRegistry::Get().RegisterClass(cls);
}


