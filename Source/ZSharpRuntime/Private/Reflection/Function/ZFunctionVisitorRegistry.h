// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFunctionVisitor.h"
#include "ZFunctionVisitorHandle.h"

namespace ZSharp
{
	class FZFunctionVisitorRegistry : public FNoncopyable
	{

		friend FZFunctionVisitorHandle;

		using ThisClass = FZFunctionVisitorRegistry;

	public:
		static const FZFunctionVisitorRegistry& Get();

	public:
		FZFunctionVisitorHandle Get(const UFunction* function, bool* native = nullptr) const;

	private:
		FZFunctionVisitorRegistry();
		~FZFunctionVisitorRegistry();

	private:
		const FZFunctionVisitor* Get(FZFunctionVisitorHandle handle) const;

	private:
		void ClearAlcSensitiveStates();

	private:
		FDelegateHandle MasterAlcUnloadedDelegate;
		
		mutable TMap<FZFunctionVisitorHandle, TUniquePtr<FZFunctionVisitor>> FunctionMap;
		
	};
}


