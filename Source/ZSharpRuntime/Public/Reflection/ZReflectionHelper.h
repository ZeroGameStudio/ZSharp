// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZRuntimeTypeLocatorWrapper;
	
	struct ZSHARPRUNTIME_API FZReflectionHelper
	{

		static FString GetUFieldAliasedName(const UField* field);
		static FString GetUFieldAssemblyName(const UField* field);
		static FString GetUFieldModuleName(const UField* field);
		static bool IsUFieldModuleMapped(const UField* field);
		static const UField* GetUFieldClosestMappedAncestor(const UField* field);
		static bool GetUFieldRuntimeTypeLocator(const UField* field, FZRuntimeTypeLocatorWrapper& outLocator);
		static bool GetFFieldClassRuntimeTypeLocator(const FFieldClass* cls, FZRuntimeTypeLocatorWrapper& outLocator);
		static bool GetFPropertyRuntimeTypeLocator(const FProperty* property, FZRuntimeTypeLocatorWrapper& outLocator);
		
	};
}


