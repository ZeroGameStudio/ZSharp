// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedEnum.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicExportedEnum : public IZExportedEnum
	{

	public:
		static FZDynamicExportedEnum* Create(UEnum* uenum);

	public:
		virtual FString GetName() const override;
		virtual FString GetModule() const override;
		virtual FString GetUnrealFieldPath() const override;
		virtual EZCallBufferSlotType GetSlotType() const override;
		virtual EZExportedEnumFlags GetFlags() const override;
		virtual FString GetUnderlyingType() const override;
		virtual void ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const override;

	private:
		explicit FZDynamicExportedEnum(UEnum* uenum);

	private:
		UEnum* Enum;
		EZExportedEnumFlags Flags;
		
	};
}


