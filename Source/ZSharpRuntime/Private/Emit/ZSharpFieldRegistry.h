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
		virtual const FZSharpClass* GetClass(const UClass* cls) const override;
		virtual const FZSharpFunction* GetFunction(const UFunction* function) const override;

	public:
		FZSharpClass& RegisterClass(const UClass* cls);
		FZSharpFunction& RegisterFunction(const UFunction* function);
		
		FZSharpClass* GetMutableClass(const UClass* cls) { return const_cast<FZSharpClass*>(GetClass(cls)); }
		FZSharpFunction* GetMutableFunction(const UFunction* function) { return const_cast<FZSharpFunction*>(GetFunction(function)); }

	private:
		FZSharpFieldRegistry();
		virtual ~FZSharpFieldRegistry() override;

	private:
		void ClearAlcSensitiveStates();

	private:
		FDelegateHandle MasterAlcUnloadedDelegate;
		
		TMap<const UClass*, TUniquePtr<FZSharpClass>> ClassRegistry;
		TMap<const UFunction*, TUniquePtr<FZSharpFunction>> FunctionRegistry;
		
	};
}


