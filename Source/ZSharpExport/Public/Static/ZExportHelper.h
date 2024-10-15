// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeName.h"

namespace ZSharp
{
	struct ZSHARPEXPORT_API FZExportHelper
	{

		static FString GetUFieldExportKey(const UField* field);
		static FZFullyExportedTypeName GetUFieldFullyExportedName(const UField* field);
		static FZFullyExportedTypeName GetFPropertyFullyExportedTypeName(const FProperty* property);

		static bool IsNameDeprecated(const FString& name);
		static bool IsFieldDeprecated(FFieldVariant field);
		static bool IsFieldEditorOnly(FFieldVariant field);

		static bool ShouldExportFieldBySettings(FFieldVariant field);
		
	};
}


