﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedEnum.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicExportedEnum : public IZExportedEnum
	{

	public:
		explicit FZDynamicExportedEnum(UEnum* uenum);

	public:
		virtual bool IsRegistered() const override { return bRegistered; }
		virtual FString GetName() const override;
		virtual FString GetModule() const override;
		virtual EZCallBufferSlotType GetSlotType() const override;
		virtual FString GetUnderlyingType() const override;
		virtual void ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const override;

	private:
		bool bRegistered;
		UEnum* Enum;
		
	};
}

