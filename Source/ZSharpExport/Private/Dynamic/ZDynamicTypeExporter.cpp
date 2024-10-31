// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicTypeExporter.h"

#include "ZDynamicallyExportedClass.h"
#include "ZDynamicallyExportedDelegate.h"
#include "ZDynamicallyExportedEnum.h"
#include "Emit/IZSharpFieldRegistry.h"
#include "Reflection/ZReflectionHelper.h"
#include "ZExportHelper.h"

void ZSharp::FZDynamicTypeExporter::Export(const TArray<FString>& assemblies)
{
	IZSharpFieldRegistry& registry = IZSharpFieldRegistry::Get();

	for (TObjectIterator<UEnum> it; it; ++it)
	{
		if (!FZExportHelper::ShouldExportFieldBySettings(*it))
		{
			continue;
		}
		
		if (!assemblies.IsEmpty() && !assemblies.Contains(FZReflectionHelper::GetFieldAssemblyName(*it)))
		{
			continue;
		}
		
		FZDynamicallyExportedEnum::Create(*it);
	}

	for (TObjectIterator<UScriptStruct> it; it; ++it)
	{
		if (!FZExportHelper::ShouldExportFieldBySettings(*it))
		{
			continue;
		}
		
		if (!assemblies.IsEmpty() && !assemblies.Contains(FZReflectionHelper::GetFieldAssemblyName(*it)))
		{
			continue;
		}
		
		FZDynamicallyExportedClass::Create(*it);
	}

	for (TObjectIterator<UClass> it; it; ++it)
	{
		if (!FZExportHelper::ShouldExportFieldBySettings(*it))
		{
			continue;
		}
		
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
		if (!FZExportHelper::ShouldExportFieldBySettings(*it))
		{
			continue;
		}
		
		if (!assemblies.IsEmpty() && !assemblies.Contains(FZReflectionHelper::GetFieldAssemblyName(*it)))
		{
			continue;
		}
		
		FZDynamicallyExportedDelegate::Create(*it);
	}
}


