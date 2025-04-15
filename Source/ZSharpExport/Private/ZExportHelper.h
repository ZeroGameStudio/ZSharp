// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedDefaultValue.h"
#include "ZFullyExportedTypeName.h"

namespace ZSharp
{
	struct ZSHARPEXPORT_API FZExportHelper
	{
		static FString GetFieldRedirectedName(FFieldVariant field);
		static FString GetFieldRedirectedFullName(FFieldVariant field);

		static FString GetFieldModuleName(FFieldVariant field);
		static FString GetFieldAssemblyName(FFieldVariant field);

		static bool ShouldExportField(FFieldVariant field);
		static const UField* GetUFieldClosestExportedAncestor(const UField* field);

		static FZFullyExportedTypeName GetFieldFullyExportedTypeName(FFieldVariant field);
		static const UEnum* GetUEnumFromProperty(const FProperty* property);
		static bool CanFPropertyBeNullInNotNullOut(const FProperty* property);
		static FZExportedDefaultValue GetParameterDefaultValue(const FProperty* parameter);

		static bool IsNameDeprecated(const FString& name);

	private:
		static FZFullyExportedTypeName GetUFieldFullyExportedTypeName(const UField* field);
		static FZFullyExportedTypeName GetFPropertyFullyExportedTypeName(const FProperty* property);
		
		static bool IsFieldModuleMapped(FFieldVariant field);
		static bool IsFieldScriptNoExport(FFieldVariant field);
		static bool IsFieldInternal(FFieldVariant field);
		static bool IsFieldDeprecated(FFieldVariant field);
		static bool IsFieldEditorOnly(FFieldVariant field);
	};
}


