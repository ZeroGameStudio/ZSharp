// Copyright Zero Games. All Rights Reserved.


#include "IZExportedTypeRegistry.h"

#include "ZExportedTypeRegistry.h"
#include "Dynamic/ZDynamicTypeExporter.h"

ZSharp::IZExportedTypeRegistry& ZSharp::IZExportedTypeRegistry::Get()
{
	return FZExportedTypeRegistry::Get();
}

void ZSharp::IZExportedTypeRegistry::ExportDynamicTypes(const TArray<FString>& assemblies)
{
	FZDynamicTypeExporter::Export(assemblies);
}


