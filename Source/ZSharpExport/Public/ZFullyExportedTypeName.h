// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZSimpleFullyExportedTypeName
	{
		FString Namespace;
		FString Name;
		bool bNullable = false;
		
		bool IsValid() const { return !!Name.Len(); }
	};
	
	struct FZFullyExportedTypeName
	{
		FString Namespace;
		FString Name;
		bool bNullable = false;
		FZSimpleFullyExportedTypeName Inner;
		FZSimpleFullyExportedTypeName Outer;

		FZSimpleFullyExportedTypeName ToSimple() const { return { Namespace, Name, bNullable }; }
		bool IsValid() const { return !!Name.Len(); }
	};
}


