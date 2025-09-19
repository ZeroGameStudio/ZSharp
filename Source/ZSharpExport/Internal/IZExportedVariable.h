// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeName.h"
#include "IZExportedMember.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedVariable : public IZExportedMember
	{
	public:
		virtual FZFullyExportedTypeName GetType() const = 0;
		virtual FString GetUnderlyingType() const = 0;
		virtual bool IsNullInNotNullOut() const = 0;
		virtual bool HasBlackConjugate() const = 0;
	};
}


