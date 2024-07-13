// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedTypeRegistry.h"
#include "IZExportedClass.h"
#include "IZExportedDelegate.h"
#include "IZExportedEnum.h"
#include "IZExportedMethod.h"
#include "IZExportedProperty.h"

namespace ZSharp
{
	class FZExportedTypeRegistry : public IZExportedTypeRegistry
	{

	public:
		static FZExportedTypeRegistry& Get();

	public:
		FZExportedTypeRegistry();

	public:
		virtual void ForeachExportedClass(TFunctionRef<void(const IZExportedClass&)> action) const override;
		virtual void ForeachExportedEnum(TFunctionRef<void(const IZExportedEnum&)> action) const override;
		virtual void ForeachExportedDelegate(TFunctionRef<void(const IZExportedDelegate&)> action) const override;

	public:
		bool RegisterClass(IZExportedClass* cls);
		bool RegisterEnum(IZExportedEnum* enm);
		bool RegisterDelegate(IZExportedDelegate* delegate);
		
		bool RegisterMixinMethod(IZExportedMethod* method);
		bool RegisterMixinProperty(IZExportedProperty* property);

		void FinishRegister();

	private:
		TMap<FString, TUniquePtr<IZExportedClass>> ClassMap;
		TMap<FString, TUniquePtr<IZExportedEnum>> EnumMap;
		TMap<FString, TUniquePtr<IZExportedDelegate>> DelegateMap;
		
		TMap<FString, TUniquePtr<IZExportedMethod>> MixinMethodMap;
		TMap<FString, TUniquePtr<IZExportedProperty>> MixinPropertyMap;

		bool bFinished;
		
	};
}


