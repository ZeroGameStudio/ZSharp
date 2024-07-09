// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedClass.h"
#include "IZExportedProperty.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicExportedClass : public IZExportedClass, public FNoncopyable
	{

	public:
		static FZDynamicExportedClass* Create(UStruct* ustruct);

	public:
		virtual FString GetName() const override;
		virtual FString GetModule() const override;
		virtual FString GetUnrealFieldPath() const override;
		virtual FString GetOuterExportName() const override;
		virtual uint16 GetConjugateRegistryId() const override;
		virtual EZExportedClassFlags GetFlags() const override;
		virtual FString GetBaseType() const override;
		virtual void ForeachProperty(TFunctionRef<void(const FString&, const IZExportedProperty&)> action) const override;

	private:
		explicit FZDynamicExportedClass(UStruct* ustruct);
		
	private:
		UStruct* Struct;
		EZExportedClassFlags Flags;
		TMap<FString, TUniquePtr<IZExportedProperty>> PropertyMap;
		
	};
}


