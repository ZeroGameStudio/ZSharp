// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeName.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedVariable
	{
	public:
		virtual ~IZExportedVariable(){}
	public:
		virtual FString GetName() const = 0;
		virtual FZFullyExportedTypeName GetType() const = 0;
		virtual FString GetUnderlyingType() const = 0;
	};
}


