// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZRuntimeTypeUri;
	
	struct ZSHARPRUNTIME_API FZReflectionHelper
	{
		static FString GetFieldRedirectedName(FFieldVariant field);
		static FString GetFieldRedirectedFullName(FFieldVariant field);

		static FString GetFieldAssemblyName(FFieldVariant field);
		static FString GetFieldModuleName(FFieldVariant field);
		static bool IsFieldModuleMapped(FFieldVariant field);
		
		static const UField* GetUFieldClosestMappedAncestor(const UField* field);
		static bool GetUFieldRuntimeTypeLocator(const UField* field, FZRuntimeTypeUri& outLocator);
		
		static bool GetFFieldClassRuntimeTypeLocator(const FFieldClass* cls, FZRuntimeTypeUri& outLocator);
		static bool GetNonContainerFPropertyRuntimeTypeLocator(const FProperty* property, FZRuntimeTypeUri& outLocator);
		
	};
}


