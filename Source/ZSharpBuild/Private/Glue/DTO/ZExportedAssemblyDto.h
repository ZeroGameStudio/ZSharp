// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedClassDto.h"
#include "ZExportedEnumDto.h"

namespace ZSharp
{
	struct FZExportedAssemblyDto
	{
		TArray<FZExportedEnumDto> Enums;
		TArray<FZExportedClassDto> Classes;
	};
}


