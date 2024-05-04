// Copyright Zero Games. All Rights Reserved.


#include "ZExportedTypeRegistry.h"

void ZSharp::FZExportedTypeRegistry::RegisterClass(IZExportedClass* cls)
{
	checkNoEntry();
}

void ZSharp::FZExportedTypeRegistry::RegisterEnum(IZExportedEnum* enm)
{
	checkNoEntry();
}

void ZSharp::FZExportedTypeRegistry::ForeachExportedClass(TFunctionRef<void(IZExportedClass&)> action) const
{
	checkNoEntry();
}

void ZSharp::FZExportedTypeRegistry::ForeachExportedEnum(TFunctionRef<void(IZExportedEnum&)> action) const
{
	checkNoEntry();
}


