// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedVariable.h"

namespace ZSharp
{
	enum class EZExportedParameterDirection : uint8
	{
		In,
		Out,
		InOut,
	};
	
	class ZSHARPRUNTIME_API IZExportedParameter : public IZExportedVariable
	{
	public:
		virtual EZExportedParameterDirection GetDirection() const = 0;
	};
}


