// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedVariable.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedParameter : public IZExportedVariable
	{
	public:
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;
	};
}


