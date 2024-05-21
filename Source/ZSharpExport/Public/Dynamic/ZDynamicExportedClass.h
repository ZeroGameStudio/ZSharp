// Copyright Zero Games. All Rights Reserved.

#pragma once
#include "IZExportedClass.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicExportedClass : public IZExportedClass
	{

	public:
		explicit FZDynamicExportedClass(UStruct* ustruct);

	public:
		virtual bool IsRegistered() const override;
		virtual FString GetName() const override;
		virtual FString GetModule() const override;
		virtual FString GetOuterExportName() const override;
		virtual uint16 GetConjugateRegistryId() const override;
		virtual EZExportedClassFlags GetFlags() const override;
		virtual FString GetBaseType() const override;

	private:
		bool bRegistered;
		UStruct* Struct;
		EZExportedClassFlags Flags;
		
	};
}


