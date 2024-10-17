// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedClass.h"
#include "IZExportedMethod.h"
#include "IZExportedProperty.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicallyExportedClass : public IZExportedClass, public FNoncopyable
	{

	public:
		static FZDynamicallyExportedClass* Create(const UStruct* ustruct);

	public:
		virtual FString GetName() const override;
		virtual FString GetModule() const override;
		virtual FString GetUnrealFieldPath() const override;
		virtual uint16 GetConjugateRegistryId() const override;
		virtual EZExportedClassFlags GetFlags() const override;
		virtual FZFullyExportedTypeName GetBaseType() const override;
		virtual void ForeachInterface(TFunctionRef<void(const FZFullyExportedTypeName&)> action) const override;
		virtual void ForeachMethod(TFunctionRef<void(const IZExportedMethod&)> action) const override;
		virtual void ForeachProperty(TFunctionRef<void(const IZExportedProperty&)> action) const override;

	private:
		explicit FZDynamicallyExportedClass(const UStruct* ustruct);
		
	private:
		const UStruct* Struct;
		EZExportedClassFlags Flags;
		TArray<FZFullyExportedTypeName> Interfaces;
		TArray<TUniquePtr<IZExportedMethod>> Methods;
		TArray<TUniquePtr<IZExportedProperty>> Properties;
		
	};
}


