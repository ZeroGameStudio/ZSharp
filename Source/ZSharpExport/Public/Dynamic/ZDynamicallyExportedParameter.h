﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedParameter.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicallyExportedParameter : public IZExportedParameter
	{

	public:
		static FZDynamicallyExportedParameter* Create(const FProperty* property);

	public:
		virtual FString GetName() const override;
		virtual FZFullyExportedTypeName GetType() const override;
		virtual EZExportedParameterFlags GetFlags() const override;

	private:
		explicit FZDynamicallyExportedParameter(const FProperty* property);

	private:
		bool IsValid() const { return GetType().IsValid(); }

	private:
		const FProperty* Property;
		EZExportedParameterFlags Flags;
		
	};
}

