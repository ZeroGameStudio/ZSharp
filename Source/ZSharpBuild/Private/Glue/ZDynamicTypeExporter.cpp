// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicTypeExporter.h"

#include "Dynamic/ZDynamicallyExportedClass.h"
#include "Dynamic/ZDynamicallyExportedDelegate.h"
#include "Dynamic/ZDynamicallyExportedEnum.h"
#include "Emit/IZSharpFieldRegistry.h"
#include "Reflection/ZReflectionHelper.h"

void ZSharp::FZDynamicTypeExporter::Export(const TArray<FString>& assemblies) const
{
	IZSharpFieldRegistry& registry = IZSharpFieldRegistry::Get();
	
	for (TObjectIterator<UEnum> it; it; ++it)
	{
		if (!assemblies.IsEmpty() && !assemblies.Contains(FZReflectionHelper::GetFieldAssemblyName(*it)))
		{
			continue;
		}
		
		FZDynamicallyExportedEnum::Create(*it);
	}

	for (TObjectIterator<UScriptStruct> it; it; ++it)
	{
		if (!assemblies.IsEmpty() && !assemblies.Contains(FZReflectionHelper::GetFieldAssemblyName(*it)))
		{
			continue;
		}
		
		FZDynamicallyExportedClass::Create(*it);
	}

	for (TObjectIterator<UClass> it; it; ++it)
	{
		if (registry.IsZSharpClass(*it))
		{
			continue;
		}
		
		if (!assemblies.IsEmpty() && !assemblies.Contains(FZReflectionHelper::GetFieldAssemblyName(*it)))
		{
			continue;
		}
		
		FZDynamicallyExportedClass::Create(*it);
	}

	for (TObjectIterator<UDelegateFunction> it; it; ++it)
	{
		if (!assemblies.IsEmpty() && !assemblies.Contains(FZReflectionHelper::GetFieldAssemblyName(*it)))
		{
			continue;
		}
		
		FZDynamicallyExportedDelegate::Create(*it);
	}
}


