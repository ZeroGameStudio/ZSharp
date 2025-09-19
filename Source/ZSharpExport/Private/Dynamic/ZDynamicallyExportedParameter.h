// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedParameter.h"

namespace ZSharp
{
	class FZDynamicallyExportedParameter : public IZExportedParameter
	{

	public:
		static FZDynamicallyExportedParameter* Create(const FProperty* property);

	public:
		virtual FString GetName() const override;
		virtual FZFullyExportedTypeName GetType() const override;
		virtual FString GetUnderlyingType() const override;
		virtual bool IsNullInNotNullOut() const override;
		virtual bool HasBlackConjugate() const override;
		virtual EZExportedParameterFlags GetFlags() const override;
		virtual FZExportedDefaultValue GetDefaultValue() const override;

	private:
		explicit FZDynamicallyExportedParameter(const FProperty* property);

	private:
		bool IsValid() const { return GetType().IsValid(); }

	private:
		const FProperty* Property;
		bool bNullInNotNullOut;
		EZExportedParameterFlags Flags;
		
	};
}


