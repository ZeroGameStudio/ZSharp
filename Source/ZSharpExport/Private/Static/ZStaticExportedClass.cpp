// Copyright Zero Games. All Rights Reserved.


#include "Static/ZStaticExportedClass.h"

#include "ZExportedTypeRegistry.h"

bool ZSharp::ZStaticExportedClass_Private::RegisterClass(IZExportedClass* cls)
{
	return FZExportedTypeRegistry::Get().RegisterClass(cls);
}


