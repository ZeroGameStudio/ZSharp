﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExportedTypeDto.h"

namespace ZSharp
{
	struct FZExportedEnumDto : FZExportedTypeDto
	{
		FString UnderlyingType;
		TMap<FString, FString> ValueMap;
	};
}

