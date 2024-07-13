// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedProperty.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicallyExportedProperty : public IZExportedProperty
	{

	public:
		static FZDynamicallyExportedProperty* Create(const FProperty* property, int32 index);

	public:
		virtual FString GetName() const override;
		virtual FString GetZCallName() const override;
		virtual FZFullyExportedTypeName GetType() const override;
		virtual EZExportedPropertyFlags GetFlags() const override;
		virtual int32 GetIndex() const override;

	private:
		explicit FZDynamicallyExportedProperty(const FProperty* property, int32 index);

	private:
		bool IsValid() const;

	private:
		const FProperty* Property;
		int32 Index;
		EZExportedPropertyFlags Flags;
		
	};
}


