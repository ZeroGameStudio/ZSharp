// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZRuntimeTypeLocatorWrapper;
	
	struct ZSHARPRUNTIME_API FZReflectionHelper
	{
		static FString GetFieldAliasedName(FFieldVariant field);
		static FString GetFieldFullAliasedName(FFieldVariant field);

		static FString GetFieldAssemblyName(FFieldVariant field);
		static FString GetFieldModuleName(FFieldVariant field);
		static bool IsFieldModuleMapped(FFieldVariant field);
		
		static const UField* GetUFieldClosestMappedAncestor(const UField* field);
		static bool GetUFieldRuntimeTypeLocator(const UField* field, FZRuntimeTypeLocatorWrapper& outLocator);
		
		static bool GetFFieldClassRuntimeTypeLocator(const FFieldClass* cls, FZRuntimeTypeLocatorWrapper& outLocator);
		static bool GetNonContainerFPropertyRuntimeTypeLocator(const FProperty* property, FZRuntimeTypeLocatorWrapper& outLocator);
		
	};
}


