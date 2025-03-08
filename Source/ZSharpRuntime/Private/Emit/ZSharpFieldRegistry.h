// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Emit/IZSharpFieldRegistry.h"

namespace ZSharp
{
	class FZSharpFieldRegistry : public IZSharpFieldRegistry, public FNoncopyable
	{

		using ThisClass = FZSharpFieldRegistry;
		
	public:
		static FZSharpFieldRegistry& Get();

	public:
		virtual const FZSharpEnum* GetEnum(const UEnum* enm) const override;
		virtual const FZSharpScriptStruct* GetScriptStruct(const UScriptStruct* scriptStruct) const override;
		virtual const FZSharpDelegate* GetDelegate(const UDelegateFunction* delegate) const override;
		virtual const FZSharpClass* GetClass(const UClass* cls) const override;
		virtual const FZSharpFunction* GetFunction(const UFunction* function) const override;

	public:
		FZSharpEnum& RegisterEnum(const UEnum* enm);
		FZSharpScriptStruct& RegisterScriptStruct(const UScriptStruct* scriptStruct);
		FZSharpDelegate& RegisterDelegate(const UDelegateFunction* delegate);
		FZSharpClass& RegisterClass(const UClass* cls);
		FZSharpFunction& RegisterFunction(const UFunction* function);

		FZSharpEnum* GetMutableEnum(const UEnum* enm) { return const_cast<FZSharpEnum*>(GetEnum(enm)); }
		FZSharpScriptStruct* GetMutableScriptStruct(const UScriptStruct* scriptStruct) { return const_cast<FZSharpScriptStruct*>(GetScriptStruct(scriptStruct)); }
		FZSharpDelegate* GetMutableDelegate(const UDelegateFunction* delegate) { return const_cast<FZSharpDelegate*>(GetDelegate(delegate)); }
		FZSharpClass* GetMutableClass(const UClass* cls) { return const_cast<FZSharpClass*>(GetClass(cls)); }
		FZSharpFunction* GetMutableFunction(const UFunction* function) { return const_cast<FZSharpFunction*>(GetFunction(function)); }

	private:
		FZSharpFieldRegistry();
		virtual ~FZSharpFieldRegistry() override;

	private:
		void ClearAlcSensitiveStates();

	private:
		FDelegateHandle MasterAlcUnloadedDelegate;

		TMap<const UEnum*, TUniquePtr<FZSharpEnum>> EnumRegistry;
		TMap<const UScriptStruct*, TUniquePtr<FZSharpScriptStruct>> ScriptStructRegistry;
		TMap<const UDelegateFunction*, TUniquePtr<FZSharpDelegate>> DelegateRegistry;
		TMap<const UClass*, TUniquePtr<FZSharpClass>> ClassRegistry;
		TMap<const UFunction*, TUniquePtr<FZSharpFunction>> FunctionRegistry;
		
	};
}


