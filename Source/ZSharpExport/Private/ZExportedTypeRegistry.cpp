// Copyright Zero Games. All Rights Reserved.


#include "ZExportedTypeRegistry.h"

ZSharp::FZExportedTypeRegistry& ZSharp::FZExportedTypeRegistry::Get()
{
	static FZExportedTypeRegistry GSingleton;

	return GSingleton;
}

ZSharp::FZExportedTypeRegistry::FZExportedTypeRegistry()
	: bFinished(false)
{
}

void ZSharp::FZExportedTypeRegistry::ForeachExportedClass(TFunctionRef<void(const IZExportedClass&)> action) const
{
	for (const auto& pair : ClassMap)
	{
		action(*pair.Value);
	}
}

void ZSharp::FZExportedTypeRegistry::ForeachExportedEnum(TFunctionRef<void(const IZExportedEnum&)> action) const
{
	for (const auto& pair : EnumMap)
	{
		action(*pair.Value);
	}
}

bool ZSharp::FZExportedTypeRegistry::RegisterClass(IZExportedClass* cls)
{
	FString key = cls->GetInnerExportName();
	if (EnumMap.Contains(key))
	{
		return false;
	}
	
	ClassMap.Emplace(key, cls);
	return true;
}

bool ZSharp::FZExportedTypeRegistry::RegisterEnum(IZExportedEnum* enm)
{
	FString key = enm->GetInnerExportName();
	if (EnumMap.Contains(key))
	{
		return false;
	}
	
	EnumMap.Emplace(key, enm);
	return true;
}

bool ZSharp::FZExportedTypeRegistry::RegisterMixinMethod(IZExportedMethod* method)
{
	checkNoEntry();
	return false;
}

bool ZSharp::FZExportedTypeRegistry::RegisterMixinProperty(IZExportedProperty* property)
{
	checkNoEntry();
	return false;
}

void ZSharp::FZExportedTypeRegistry::FinishRegister()
{
	check(!bFinished);
	
	bFinished = true;
}


