// Copyright Zero Games. All Rights Reserved.


#include "ZExportedTypeRegistry.h"

ZSharp::FZExportedTypeRegistry& ZSharp::FZExportedTypeRegistry::Get()
{
	static FZExportedTypeRegistry GSingleton;

	return GSingleton;
}

void ZSharp::FZExportedTypeRegistry::ForeachExportedClass(TFunctionRef<void(IZExportedClass&)> action) const
{
	checkNoEntry();
}

void ZSharp::FZExportedTypeRegistry::ForeachExportedEnum(TFunctionRef<void(IZExportedEnum&)> action) const
{
	for (const auto& pair : EnumMap)
	{
		action(*pair.Value);
	}
}

bool ZSharp::FZExportedTypeRegistry::RegisterClass(IZExportedClass* cls)
{
	checkNoEntry();
	return false;
}

bool ZSharp::FZExportedTypeRegistry::RegisterEnum(IZExportedEnum* enm)
{
	FString key = FString::Printf(TEXT("%s.%s"), *enm->GetModule(), *enm->GetName());
	if (EnumMap.Contains(key))
	{
		return false;
	}
	
	EnumMap.Emplace(key, enm);
	return true;
}


