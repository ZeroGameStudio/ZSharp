// Copyright Zero Games. All Rights Reserved.

#include "ZDynamicTypeExporter.h"

#include "ZDynamicallyExportedClass.h"
#include "ZDynamicallyExportedDelegate.h"
#include "ZDynamicallyExportedEnum.h"
#include "ZExportHelper.h"
#include "ZUEnumUnderlyingTypeLookup.h"

void ZSharp::FZDynamicTypeExporter::Export(const TArray<FString>& assemblies)
{
	auto shouldExportAssembly = [&](const UField* field)
	{
		return assemblies.IsEmpty() || assemblies.Contains(FZExportHelper::GetFieldAssemblyName(field));
	};

	FZUEnumUnderlyingTypeLookup::Get().InvalidateCache();
	
	for (TObjectIterator<UEnum> it; it; ++it)
	{
		if (shouldExportAssembly(*it))
		{
			FZDynamicallyExportedEnum::Create(*it);
		}
	}

	for (TObjectIterator<UScriptStruct> it; it; ++it)
	{
		if (shouldExportAssembly(*it))
		{
			FZDynamicallyExportedClass::Create(*it);
		}
	}

	for (TObjectIterator<UClass> it; it; ++it)
	{
		if (shouldExportAssembly(*it))
		{
			FZDynamicallyExportedClass::Create(*it);
		}
	}

	for (TObjectIterator<UDelegateFunction> it; it; ++it)
	{
		if (shouldExportAssembly(*it))
		{
			FZDynamicallyExportedDelegate::Create(*it);
		}
	}
}


