// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicTypeExporter.h"

#include "Dynamic/ZDynamicallyExportedClass.h"
#include "Dynamic/ZDynamicallyExportedDelegate.h"
#include "Dynamic/ZDynamicallyExportedEnum.h"

void ZSharp::FZDynamicTypeExporter::Export() const
{
	for (TObjectIterator<UEnum> it; it; ++it)
	{
		FZDynamicallyExportedEnum::Create(*it);
	}

	for (TObjectIterator<UScriptStruct> it; it; ++it)
	{
		FZDynamicallyExportedClass::Create(*it);
	}

	for (TObjectIterator<UClass> it; it; ++it)
	{
		FZDynamicallyExportedClass::Create(*it);
	}

	for (TObjectIterator<UFunction> it; it; ++it)
	{
		FZDynamicallyExportedDelegate::Create(*it);
	}
}


