﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedMember.h"
#include "ZCall/ZCallBuffer.h"

namespace ZSharp
{
	class IZExportedMethod;
	
	class ZSHARPEXPORT_API IZExportedType : public IZExportedMember
	{
	public:
		virtual FString GetModule() const = 0;
		virtual FString GetUnrealFieldPath() const = 0;
		virtual EZCallBufferSlotType GetSlotType() const = 0;
		virtual FString GetExportKey() const;
	};
}



