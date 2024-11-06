// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedDefaultValue.h"
#include "ZFullyExportedTypeName.h"

namespace ZSharp
{
	struct ZSHARPEXPORT_API FZExportHelper
	{

		static FString GetUFieldExportKey(const UField* field);
		static FZFullyExportedTypeName GetUFieldFullyExportedName(const UField* field);
		static FZFullyExportedTypeName GetFPropertyFullyExportedTypeName(const FProperty* property);
		static const UEnum* GetUEnumFromProperty(const FProperty* property);
		static bool CanFPropertyBeNullInNotNullOut(const FProperty* property);

		static bool IsNameDeprecated(const FString& name);
		static bool IsFieldDeprecated(FFieldVariant field);
		static bool IsFieldEditorOnly(FFieldVariant field);

		static bool ShouldExportFieldBySettings(FFieldVariant field);

		static FZExportedDefaultValue GetParameterDefaultValue(const FProperty* parameter);
		
	};
}


