// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct ZSHARPEXPORTRUNTIME_API FZSharpExportHelpers
	{
		static FString GetUFieldAliasedName(const UField* field);
		static FString GetUFieldModuleName(const UField* field);
		static bool IsUFieldModuleMapped(const UField* field);
		static FString GetUFieldOuterExportName(const UField* field);
		static FString GetUFieldInnerExportName(const UField* field);
	};
}


